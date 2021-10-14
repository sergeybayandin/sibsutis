#include "../include/convolution.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t anum, bnum;
	printf("Введите n, m: "); scanf("%zu %zu", &anum, &bnum);
	const size_t maxnum = (anum > bnum) ? anum : bnum;
	const size_t cnum = anum + bnum - 1;
	int* a = calloc(maxnum, sizeof(*a));
	int* b = calloc(maxnum, sizeof(*b));
	int* c = calloc(cnum, sizeof(*c));
	if (a == NULL || b == NULL || c == NULL) {
		fprintf(stderr, "Не удалось выделить память\n");
		return EXIT_FAILURE;
	}
	printf("Введите a: ");
	for (size_t i = 0; i < anum; ++i)
		scanf("%d", a + i);
	printf("Введите b: ");
	for (size_t i = 0; i < bnum; ++i)
		scanf("%d", b + i);
	const size_t k = default_convolution(a, b, c, maxnum, cnum);
	printf("c(%zu):\t", k);
	for (size_t i = 0; i < cnum; ++i)
		printf("%d\t", c[i]);
	puts("");
	return EXIT_SUCCESS;
}
