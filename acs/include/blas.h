#ifndef __BLAS_H__
#define __BLAS_H__

#include <stddef.h>

void dgemm_o0(const double*, const double*, double*, size_t);
void dgemm_o1(const double*, const double*, double*, size_t);
void dgemm_o2(const double*, const double*, double*, size_t, size_t);
void dgemm_o3_sse(const double*, const double*, double*, size_t);

#endif // __BLAS_H__
