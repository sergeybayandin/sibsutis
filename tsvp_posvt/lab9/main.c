#include "../include/convolution.h"
#include "../include/iocomplex.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t anum, bnum;
	printf("Введите n, m: "); scanf("%zu %zu", &anum, &bnum);
	const size_t cnum = anum + bnum;
	carray_t a = calloc(cnum, sizeof *a);
	carray_t b = calloc(cnum, sizeof *b);
	carray_t c = calloc(cnum, sizeof *c);
	if (a == NULL || b == NULL || c == NULL) {
		fprintf(stderr, "Не удалось выделить память\n");
		return EXIT_FAILURE;
	}
	int val;
	printf("Введите a: ");
	for (size_t i = 0; i < anum; ++i) {
		scanf("%d", &val);
		a[i] = val;
	}
	printf("Введите b: ");
	for (size_t i = 0; i < bnum; ++i) {
		scanf("%d", &val);
		b[i] = val;
	}
	const size_t k = cft_convolution(a, b, c, cnum, csfft, cisfft);
	printf("c(%zu):\n", k);
	print_carray("(%f,%f)\n", c, cnum - 1);
	return EXIT_SUCCESS;
}
