#include "../include/tsvp_posvt/convolution.h"
#include "../include/tsvp_posvt/iocomplex.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t n, m;
	printf("Введите n, m: "); scanf("%zu %zu", &n, &m);
	size_t max = n > m ? n : m;
	size_t power2 = 1;
	for (; power2 < max; power2 *= 2);
	power2 *= 2;
	carray_t a = calloc(power2, sizeof *a);
	carray_t b = calloc(power2, sizeof *b);
	carray_t c = calloc(power2, sizeof *c);
	if (a == NULL || b == NULL || c == NULL) {
		fprintf(stderr, "Не удалось выделить память\n");
		return EXIT_FAILURE;
	}
	int val;
	printf("Введите a: ");
	for (size_t i = 0; i < n; ++i) {
		scanf("%d", &val);
		a[i] = val;
	}
	printf("Введите b: ");
	for (size_t i = 0; i < m; ++i) {
		scanf("%d", &val);
		b[i] = val;
	}
	const size_t k = cft_convolution(a, b, c, power2, cfft, cifft);
	printf("c(%zu):\n", k);
	print_carray("(%f,%f)\n", c, max);
	return EXIT_SUCCESS;
}
