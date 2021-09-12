#include <stdio.h>
#include <stdlib.h>

void convolution(int* c, int* a, int* b, size_t num)
{
	for (size_t i = 0; i < num; ++i)
		for (size_t j = 0; j <= i; ++j)
			c[i] += a[j] * b[i - j];
}

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
	printf("a: ");
	for (size_t i = 0; i < n; ++i)
		scanf("%d", a + i);
	printf("b: ");
	for (size_t i = 0; i < m; ++i)
		scanf("%d", b + i);
	convolution(c, a, b, max);
	printf("c: ");
	for (size_t i = 0; i < max; ++i)
		printf("%d\t", c[i]);
	puts("");
	return EXIT_SUCCESS;
}
