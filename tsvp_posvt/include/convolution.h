#ifndef __CONVOLUTION_H__
#define __CONVOLUTION_H__

#include "fourier_transform.h"

typedef size_t (*ft_t)(const carray_t, carray_t, size_t);
typedef size_t (*ift_t)(const carray_t, carray_t, size_t);

size_t default_convolution(const int*, const int*, int*, size_t, size_t);
size_t cft_convolution(const carray_t, const carray_t, carray_t, size_t, ft_t, ift_t);

#endif // __CONVOLUTION_H__
