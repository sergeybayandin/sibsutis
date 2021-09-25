#include "../include/tsvp_posvt/fourier_transform.h"
#include "../include/tsvp_posvt/iocomplex.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t n; 
	printf("Введите размер массива: "); scanf("%zu", &n); getchar();
	double complex* fs = malloc(n * sizeof *fs);
	double complex* as = malloc(n * sizeof *as);
	if (fs == NULL || as == NULL) {
		fprintf(stderr, "Не удалось выделить память под массивы\n");
		return EXIT_FAILURE;
	}
	puts("Введите числа:");
	if (get_carray(fs, n) == -1) {
		fprintf(stderr, "Ошибка ввода\n");
		return EXIT_FAILURE;
	}
	size_t c = cdft(fs, as, n);
	printf("Прямое ДПФ(%zu):\n", c); print_carray("(%f, %f)\n", as, n); puts("");
	c = cidft(as, fs, n);
	printf("Обратное ДПФ(%zu):\n", c); print_carray("(%f, %f)\n", fs, n); puts("");
	return EXIT_SUCCESS;
}
