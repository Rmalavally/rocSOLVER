/* ************************************************************************
 * Copyright 2016 Advanced Micro Devices, Inc.
 *
 * ************************************************************************/

#pragma once
#ifndef _CBLAS_INTERFACE_
#define _CBLAS_INTERFACE_

#include "rocblas.h"

/*!\file
 * \brief provide template functions interfaces to CBLAS and LAPACK interfaces, it is
 * only used for testing not part of the GPU library
 */

template <typename T>
void cblas_iamax(rocblas_int n, const T *x, rocblas_int incx,
                 rocblas_int *result);

template <typename T>
void cblas_iamin(rocblas_int n, const T *x, rocblas_int incx,
                 rocblas_int *result);

template <typename T1, typename T2>
void cblas_asum(rocblas_int n, const T1 *x, rocblas_int incx, T2 *result);

template <typename T>
void cblas_axpy(rocblas_int n, const T alpha, T *x, rocblas_int incx, T *y,
                rocblas_int incy);

template <typename T>
void cblas_copy(rocblas_int n, T *x, rocblas_int incx, T *y, rocblas_int incy);

template <typename T>
void cblas_dot(rocblas_int n, const T *x, rocblas_int incx, const T *y,
               rocblas_int incy, T *result);

template <typename T1, typename T2>
void cblas_nrm2(rocblas_int n, const T1 *x, rocblas_int incx, T2 *result);

template <typename T>
void cblas_scal(rocblas_int n, const T alpha, T *x, rocblas_int incx);

template <typename T>
void cblas_swap(rocblas_int n, T *x, rocblas_int incx, T *y, rocblas_int incy);

template <typename T>
void cblas_gemv(rocblas_operation transA, rocblas_int m, rocblas_int n, T alpha,
                T *A, rocblas_int lda, T *x, rocblas_int incx, T beta, T *y,
                rocblas_int incy);

template <typename T>
void cblas_symv(rocblas_fill uplo, rocblas_int n, T alpha, T *A,
                rocblas_int lda, T *x, rocblas_int incx, T beta, T *y,
                rocblas_int incy);

template <typename T>
void cblas_ger(rocblas_int m, rocblas_int n, T alpha, T *x, rocblas_int incx,
               T *y, rocblas_int incy, T *A, rocblas_int lda);

template <typename T>
void cblas_syr(rocblas_fill uplo, rocblas_int n, T alpha, T *x,
               rocblas_int incx, T *A, rocblas_int lda);

template <typename T>
void cblas_hemv(rocblas_fill uplo, rocblas_int n, T alpha, T *A,
                rocblas_int lda, T *x, rocblas_int incx, T beta, T *y,
                rocblas_int incy);

template <typename T>
void cblas_gemm(rocblas_operation transA, rocblas_operation transB,
                rocblas_int m, rocblas_int n, rocblas_int k, T alpha, T *A,
                rocblas_int lda, T *B, rocblas_int ldb, T beta, T *C,
                rocblas_int ldc);

template <typename T>
void cblas_trsm(rocblas_side side, rocblas_fill uplo, rocblas_operation transA,
                rocblas_diagonal diag, rocblas_int m, rocblas_int n, T alpha,
                const T *A, rocblas_int lda, T *B, rocblas_int ldb);

template <typename T>
void cblas_potf2(rocblas_fill uplo, rocblas_int n, T *A,
                        rocblas_int lda, rocblas_int *info);

template <typename T>
void cblas_potrf(rocblas_fill uplo, rocblas_int n, T *A,
                        rocblas_int lda, rocblas_int *info);


template <typename T>
void cblas_getf2(rocblas_int m, rocblas_int n, T *A, rocblas_int lda,
                        rocblas_int *ipiv, rocblas_int *info);

template <typename T>
rocblas_int cblas_trtri(char uplo, char diag, rocblas_int n, T *A,
                        rocblas_int lda);

template <typename T>
void cblas_trmm(rocblas_side side, rocblas_fill uplo, rocblas_operation transA,
                rocblas_diagonal diag, rocblas_int m, rocblas_int n, T alpha,
                const T *A, rocblas_int lda, T *B, rocblas_int ldb);

template <typename T>
void cblas_getrf(rocblas_int m, rocblas_int n, T *A, rocblas_int lda,
                        rocblas_int *ipiv, rocblas_int *info);

template <typename T>
rocblas_int cblas_getrs(char trans, rocblas_int n, rocblas_int nrhs, T *A,
                        rocblas_int lda, rocblas_int *ipiv, T *B,
                        rocblas_int ldb);

template <typename T>
rocblas_int cblas_potrf(char uplo, rocblas_int m, T *A, rocblas_int lda);

template <typename T>
void cblas_larfg(rocblas_int n, T* alpha, T* x, rocblas_int incx, T* tau);

template <typename T>
void cblas_larf(char side, rocblas_int m, rocblas_int n, T* x, rocblas_int incx, T* alpha, T *A, rocblas_int lda, T *work);

template <typename T>
void cblas_larft(char direct, char storev, rocblas_int n, rocblas_int k, T* V, rocblas_int ldv, T* tau, T *F, rocblas_int ldt);

template <typename T>
void cblas_larfb(char side, char trans, char direct, char storev, rocblas_int m, rocblas_int n, rocblas_int k, 
                 T* V, rocblas_int ldv, T *F, rocblas_int ldt, T *A, rocblas_int lda, T *W, rocblas_int ldw);

template <typename T>
void cblas_geqr2(rocblas_int m, rocblas_int n, T* A, rocblas_int lda, T *ipiv, T *work);

template <typename T>
void cblas_geqrf(rocblas_int m, rocblas_int n, T* A, rocblas_int lda, T *ipiv, T *work, rocblas_int sizeW);

template <typename T>
void cblas_gelq2(rocblas_int m, rocblas_int n, T* A, rocblas_int lda, T *ipiv, T *work);

template <typename T>
void cblas_gelqf(rocblas_int m, rocblas_int n, T* A, rocblas_int lda, T *ipiv, T *work, rocblas_int sizeW);

template <typename T>
void cblas_laswp(rocblas_int n, T* A, rocblas_int lda, rocblas_int k1, rocblas_int k2, rocblas_int *ipiv, rocblas_int inc);

template <typename T>
void cblas_org2r(rocblas_int m, rocblas_int n, rocblas_int k, T *A, rocblas_int lda, T *Ipiv, T *work);

template <typename T>
void cblas_orgqr(rocblas_int m, rocblas_int n, rocblas_int k, T *A, rocblas_int lda, T *Ipiv, T *work);

template <typename T>
void cblas_orgl2(rocblas_int m, rocblas_int n, rocblas_int k, T *A, rocblas_int lda, T *Ipiv, T *work);

template <typename T>
void cblas_orglq(rocblas_int m, rocblas_int n, rocblas_int k, T *A, rocblas_int lda, T *Ipiv, T *work);

template <typename T>
void cblas_orgbr(char storev, rocblas_int m, rocblas_int n, rocblas_int k, T *A, rocblas_int lda, T *Ipiv, T *work, rocblas_int size_w);

template <typename T>
void cblas_gebrd(rocblas_int m, rocblas_int n, T *A, rocblas_int lda, T *D, T *E, T *tauq, T *taup, T *work, rocblas_int size_w);

template <typename T>
void cblas_orm2r(rocblas_side side, rocblas_operation trans, rocblas_int m, rocblas_int n, rocblas_int k, T *A, rocblas_int lda, T *Ipiv, 
                 T *C, rocblas_int ldc, T *work);

template <typename T>
void cblas_ormqr(rocblas_side side, rocblas_operation trans, rocblas_int m, rocblas_int n, rocblas_int k, T *A, rocblas_int lda, T *Ipiv, 
                 T *C, rocblas_int ldc, T *work, rocblas_int sizeW);


#endif /* _CBLAS_INTERFACE_ */
