#ifndef __FOURIER_TRANSFORM_H__
#define __FOURIER_TRANSFORM_H__

#include "complex_types.h"
#include <stddef.h>

// Descreate fourier transform
size_t cdft(const carray_t, carray_t, size_t);
size_t cidft(const carray_t, carray_t, size_t);

// Semi-fast fourier transform
size_t csfft(const carray_t, carray_t, size_t);
size_t cisfft(const carray_t, carray_t, size_t);

// Fast fourier transform
size_t cfft(const carray_t, carray_t, size_t);
size_t cifft(const carray_t, carray_t, size_t);

#endif //__FOURIER_TRANSFORM_H__
