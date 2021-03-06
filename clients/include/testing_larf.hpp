/* ************************************************************************
 * Copyright 2018 Advanced Micro Devices, Inc.
 * ************************************************************************ */

#include <cmath> // std::abs
#include <fstream>
#include <iostream>
#include <limits> // std::numeric_limits<T>::epsilon();
#include <stdlib.h>
#include <string>
#include <vector>

#include "arg_check.h"
#include "cblas_interface.h"
#include "norm.h"
#include "rocblas_test_unique_ptr.hpp"
#include "rocsolver.hpp"
#include "unit.h"
#include "utility.h"
#ifdef GOOGLE_TEST
#include <gtest/gtest.h>
#endif

#define ERROR_EPS_MULTIPLIER 5000
// AS IN THE ORIGINAL ROCSOLVER TEST UNITS, WE CURRENTLY USE A HIGH TOLERANCE 
// AND THE MAX NORM TO EVALUATE THE ERROR. THIS IS NOT "NUMERICALLY SOUND"; 
// A MAJOR REFACTORING OF ALL UNIT TESTS WILL BE REQUIRED.  

using namespace std;

template <typename T> 
rocblas_status testing_larf(Arguments argus) {
    rocblas_int M = argus.M;
    rocblas_int N = argus.N;
    rocblas_int incx = argus.incx;
    rocblas_int lda = argus.lda;
    char sideC = argus.side_option;    
    int hot_calls = argus.iters;
    rocblas_side side;

    std::unique_ptr<rocblas_test::handle_struct> unique_ptr_handle(new rocblas_test::handle_struct);
    rocblas_handle handle = unique_ptr_handle->handle;

    // check invalid size and quick return
    if (N < 1 || M < 1 || lda < M || !incx) {
        auto dx_managed = rocblas_unique_ptr{rocblas_test::device_malloc(sizeof(T)), rocblas_test::device_free};
        T *dx = (T *)dx_managed.get();

        auto dalpha_managed = rocblas_unique_ptr{rocblas_test::device_malloc(sizeof(T)), rocblas_test::device_free};
        T *dalpha = (T *)dalpha_managed.get();

        auto dA_managed = rocblas_unique_ptr{rocblas_test::device_malloc(sizeof(T)), rocblas_test::device_free};
        T *dA = (T *)dA_managed.get();
        
        if (!dx || !dalpha || !dA) {
            PRINT_IF_HIP_ERROR(hipErrorOutOfMemory);
            return rocblas_status_memory_error;
        }
        
        return rocsolver_larf<T>(handle, side, M, N, dx, incx, dalpha, dA, lda);
    }

    rocblas_int sizex = 1;
    rocblas_int order, sizew;
    if (sideC == 'L') {
        order = M;
        sizew = N;
        side = rocblas_side_left;    
        sizex += (M - 1) * abs(incx);
    } else if (sideC == 'R') {
        order = N;
        sizew = M;
        side = rocblas_side_right;
        sizex += (N - 1) * abs(incx);
    } else {
        throw runtime_error("Unsupported side option.");
    }
    rocblas_int sizeA = lda * N;    

    // Naming: dK is in GPU (device) memory. hK is in CPU (host) memory
    vector<T> hx(sizex);
    vector<T> hA(sizeA);
    vector<T> hA_r(sizeA);
    vector<T> hw(sizew);
    T halpha;

    auto dx_managed = rocblas_unique_ptr{rocblas_test::device_malloc(sizeof(T) * sizex), rocblas_test::device_free};
    T *dx = (T *)dx_managed.get();
    auto dalpha_managed = rocblas_unique_ptr{rocblas_test::device_malloc(sizeof(T)), rocblas_test::device_free};
    T *dalpha = (T *)dalpha_managed.get();
    auto dA_managed = rocblas_unique_ptr{rocblas_test::device_malloc(sizeof(T) * sizeA), rocblas_test::device_free};
    T *dA = (T *)dA_managed.get();
  
    if (!dx || !dalpha || !dA) {
        PRINT_IF_HIP_ERROR(hipErrorOutOfMemory);
        return rocblas_status_memory_error;
    }

    //initialize full random inputs, all entries in [1, 10]
    rocblas_init<T>(hx.data(), 1, order, abs(incx));
    rocblas_init<T>(hA.data(), M, N, lda);
    rocblas_init<T>(&halpha, 1, 1, 1);

    // copy data from CPU to device
    CHECK_HIP_ERROR(hipMemcpy(dx, hx.data(), sizeof(T) * sizex, hipMemcpyHostToDevice));
    CHECK_HIP_ERROR(hipMemcpy(dA, hA.data(), sizeof(T) * sizeA, hipMemcpyHostToDevice));
    CHECK_HIP_ERROR(hipMemcpy(dalpha, &halpha, sizeof(T), hipMemcpyHostToDevice));

    double gpu_time_used, cpu_time_used;
    double error_eps_multiplier = ERROR_EPS_MULTIPLIER;
    double eps = std::numeric_limits<T>::epsilon();
    double max_err_1 = 0.0, max_val = 0.0;
    double diff;

/* =====================================================================
           ROCSOLVER
    =================================================================== */  
    if (argus.unit_check || argus.norm_check) {
        //GPU lapack
        CHECK_ROCBLAS_ERROR(rocsolver_larf<T>(handle, side, M, N, dx, incx, dalpha, dA, lda));
        
        //copy output from device to cpu
        CHECK_HIP_ERROR(hipMemcpy(hA_r.data(), dA, sizeof(T) * sizeA, hipMemcpyDeviceToHost));

        //CPU lapack
        cpu_time_used = get_time_us();
        cblas_larf<T>(sideC, M, N, hx.data(), incx, &halpha, hA.data(), lda, hw.data());
        cpu_time_used = get_time_us() - cpu_time_used;

        // +++++++++ Error Check +++++++++++++
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                diff = fabs(hA[i + j * lda]);
                max_val = max_val > diff ? max_val : diff;
                diff = hA[i + j * lda];
                diff = fabs(hA_r[i + j * lda] - diff);
                max_err_1 = max_err_1 > diff ? max_err_1 : diff;
            }
        }

        max_err_1 = max_err_1 / max_val;
        
        if(argus.unit_check)
            getf2_err_res_check<T>(max_err_1, M, N, error_eps_multiplier, eps);
    }
 

    if (argus.timing) {
        // GPU rocBLAS
        int cold_calls = 2;

        for(int iter = 0; iter < cold_calls; iter++)
            rocsolver_larf<T>(handle, side, M, N, dx, incx, dalpha, dA, lda);
        gpu_time_used = get_time_us();
        for(int iter = 0; iter < hot_calls; iter++)
            rocsolver_larf<T>(handle, side, M, N, dx, incx, dalpha, dA, lda);
        gpu_time_used = (get_time_us() - gpu_time_used) / hot_calls;       

        // only norm_check return an norm error, unit check won't return anything
        cout << "M,N,incx,lda,gpu_time(us),cpu_time(us)";

        if (argus.norm_check)
            cout << ",norm_error_host_ptr";

        cout << endl;
        cout << M << "," << N << "," << incx << "," << lda << "," << gpu_time_used << "," << cpu_time_used;

        if (argus.norm_check)
            cout << "," << max_err_1;

        cout << endl;
    }
    
    return rocblas_status_success;
}

#undef ERROR_EPS_MULTIPLIER
