#include "../include/blas.h"

void dgemm_o0(double** a, double** b, double** c, size_t n)
{
	size_t i, j, k;
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			c[i][j] = 0;
			for (k = 0; k < n; ++k)
				c[i][j] += a[i][k] * b[k][j];
		}
	}
}

void dgemm_o1(double** a, double** b, double** c, size_t n)
{
	size_t i, j, k;
	for (i = 0; i < n; ++i) {
		for (k = 0; k < n; ++k) {
			c[i][j] = 0;
			for (j = 0; j < n; ++j)
				c[i][j] += a[i][k] * b[k][j];
		}
	}
}

void dgemm_o2(double* a, double* b, double* c, size_t n, size_t bs)
{
	double* a0, *c0, *b0;
	size_t i, j, k, c, t, s;
	for (i = 0; i < n; i += bs) {
		for (j = 0; j < n; j += bs) {
			for (k = 0; k < n; k += bs) {
				a0 = a + i * n + k;
				c0 = c + i * n + j;
				for (c = 0; c < bs; ++c, c0 += n, a0 += n) {
					b0 = b + k * n + j;
					for (t = 0; t < bs; ++t, b0 += n)
						for (s = 0; s < bs; ++s)
							c0[s] += a0[t] * b0[s];
				}
			}
		}
	}
}
