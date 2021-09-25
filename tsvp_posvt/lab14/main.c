#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

//uncorrect
void ford_bellman(int** g, int* d, size_t n, size_t v)
{
	memset(d, CHAR_MAX, n * sizeof *d); d[v] = 0;
	int relaxed = 0;
	while (!relaxed) {
		relaxed = 1;
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				const int w = d[i] + g[i][j];
				if (d[j] > w) {
					d[j] = w;
					relaxed = 0;
				}
			}
		}
	}
}

int main()
{
	size_t n;
	printf("Введите количество вершин: "); scanf("%zu", &n);
	int** g = malloc(n * sizeof *g);
	if (g == NULL) {
		fprintf(stderr, "Не удалось выделить память под граф\n");
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < n; ++i) {
		if ((g[i] = malloc(n * sizeof *g[i])) == NULL) {
			fprintf(stderr, "Не удалось выделить память под граф\n");
			return EXIT_FAILURE;
		}
		for (size_t j = 0; j < n; ++j)
			scanf("%d", g[i] + j);
	}
	int* d = malloc(n * sizeof *d);
	if (d == NULL) {
		fprintf(stderr, "Не удалось выделить память под массив кратчайших путей\n");
		return EXIT_FAILURE;
	}
	size_t v; 
	printf("Введите вершину: "); scanf("%zu", &v);
	ford_bellman(g, d, n, v);
	for (size_t i = 0; i < n; ++i)
		printf("%d\t", d[i]);
	puts("");
	return EXIT_SUCCESS;
}
