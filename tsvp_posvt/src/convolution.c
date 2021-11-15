#include "../include/convolution.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

size_t default_convolution(const int* a, const int* b, int* c, size_t abnum, size_t cnum)
{
	size_t n = 0, i, j;
	for (i = 0; i < abnum; ++i) {
		c[i] = 0;
		for (j = 0; j <= i; ++j, ++n)
			c[i] += a[j] * b[i - j];
	}
	for (; i < cnum; ++i) {
		c[i] = 0;
		for (j = i - abnum + 1; j < abnum; ++j, ++n)
			c[i] += a[j] * b[i - j];
	}
	return n;
}

size_t cft_convolution(const carray_t a, const carray_t b, carray_t c, size_t num, ft_t ft, ift_t ift)
{
	size_t n = 0;
	carray_t fa, fb, fc;
	if ((fa = calloc(num, sizeof *fa)) == NULL)
		return n;
	if ((fb = calloc(num, sizeof *fb)) == NULL) {
		free(fa);
		return n;
	}
	if ((fc = calloc(num, sizeof *fc)) == NULL) {
		free(fa); free(fb);
		return n;
	}
	n += ft(a, fa, num);
	size_t i;
	puts("fa:");
	for (i = 0; i < num; ++i)
		printf("(%f, %f)\t", creal(fa[i]), cimag(fa[i]));
	n += ft(b, fb, num);	
	puts("\nfb:");
	for (i = 0; i < num; ++i)
		printf("(%f, %f)\t", creal(fb[i]), cimag(fb[i]));
	puts("");
	for (i = 0; i < num; ++i, n += 2)
		fc[i] = num * fa[i] * fb[i];
	free(fa); free(fb);
	n += ift(fc, c, num);
	free(fc);
	return n;
}
