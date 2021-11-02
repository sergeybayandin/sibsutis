#include "../include/blas.h"
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <string.h>
#include <stdio.h>

void dgemm_o0(const double* a, const double* b, double* c, size_t n)
{
	size_t i, j, k, s, r;
	for (i = 0; i < n; ++i) {	
		r = i * n;
		for (j = 0; j < n; ++j) {
			k = r + j;
			for (s = 0; s < n; ++s)
				c[k] += a[r + s] * b[s * n + j];
		}
	}
}

void dgemm_o1(const double* a, const double* b, double* c, size_t n)
{
	size_t i, j, k, s, r, m;
	for (i = 0; i < n; ++i) {
		r = i * n;
		for (s = 0; s < n; ++s) {
			k = r + s; m = s * n;
			for (j = 0; j < n; ++j)
				c[k] += a[k] * b[m + j];
		}
	}
}

// __m128d ~ double[2]

void dgemm_o3_sse(const double* a, const double* b, double* c, size_t n) 
{
	__m128d x;
	const __m128d* _b;
	size_t i, j, k, ndiv2 = n / (sizeof x / sizeof *a);
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			x = _mm_setzero_pd();
			_b = (const __m128d*)(b + j * n);
			for (k = 0; k < ndiv2; ++k)
				x = _mm_add_pd(x, _b[k]);
			x = _mm_mul_pd(x, _mm_set1_pd(a[i * n + j]));
			c[i * n + j] = x[0] + x[1];
		}
	}
}

void dgemm_o2(const double* a, const double* b, double* c, size_t n, size_t bs)
{
	const double* a0, *b0;
	double* c0;
	size_t i, j, k, m, t, s;
	for (i = 0; i < n; i += bs) {
		for (j = 0; j < n; j += bs) {
			for (k = 0; k < n; k += bs) {
				for (m = 0, c0 = c + i * n + j,
						a0 = a + i * n + k; m < bs; ++m, c0 += n, a0 += n) {
					for (t = 0, b0 = b + k * n + j; t < bs; ++t, b0 += n)
						for (s = 0; s < bs; ++s)
							c0[s] += a0[t] * b0[s];
				}
			}
		}
	}
}
