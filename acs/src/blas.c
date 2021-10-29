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
