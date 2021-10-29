#ifndef __BLAS_H__
#define __BLAS_H__

#include <stddef.h>

void dgemm_o0(double**, double**, double**, size_t);
void dgemm_o1(double**, double**, double**, size_t);
void dgemm_o2(double**, double**, double**, size_t, size_t);

#endif // __BLAS_H__
