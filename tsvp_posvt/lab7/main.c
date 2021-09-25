#include "../include/tsvp_posvt/convolution.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t n, m;
	printf("Введите n, m: "); scanf("%zu %zu", &n, &m);
	const size_t max = (n > m) ? n : m;
	int* a = calloc(max, sizeof(*a));
	int* b = calloc(max, sizeof(*b));
	int* c = calloc(max, sizeof(*c));
	if (a == NULL || b == NULL || c == NULL) {
		fprintf(stderr, "Не удалось выделить память\n");
		return EXIT_FAILURE;
	}
	printf("Введите a: ");
	for (size_t i = 0; i < n; ++i)
		scanf("%d", a + i);
	printf("Введите b: ");
	for (size_t i = 0; i < m; ++i)
		scanf("%d", b + i);
	const size_t k = default_convolution(a, b, c, max);
	printf("c(%zu):\t", k);
	for (size_t i = 0; i < max; ++i)
		printf("%d\t", c[i]);
	puts("");
	return EXIT_SUCCESS;
}
