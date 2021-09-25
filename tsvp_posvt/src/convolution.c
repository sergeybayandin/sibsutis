#include "../../include/tsvp_posvt/convolution.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

size_t default_convolution(const int* a, const int* b, int* c, size_t num)
{
	size_t n = 0;
	for (size_t i = 0; i < num; ++i) {
		c[i] = 0;
    for (size_t j = 0; j <= i; ++j, ++n)
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
	n += ft(b, fb, num);
	for (size_t i = 0; i < num; ++i)
		fc[i] = num * fa[i] * fb[i];
	free(fa); free(fb);
	n += ift(fc, c, num);
	free(fc);
	return n;
}
