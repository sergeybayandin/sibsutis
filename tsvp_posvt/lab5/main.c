#include "../include/tsvp_posvt/fourier_transform.h"
#include "../include/tsvp_posvt/iocomplex.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t num; 
	printf("Введите размер массива: "); scanf("%zu", &num); getchar();
	carray_t fs = malloc(num * sizeof *fs);
	carray_t as = malloc(num * sizeof *as);
	if (fs == NULL || as == NULL) {
		fprintf(stderr, "Не удалось выделить память под массивы\n");
		return EXIT_FAILURE;
	}
	puts("Введите числа:");
	if (get_carray(fs, num) == -1) {
		fprintf(stderr, "Ошибка ввода\n");
		return EXIT_FAILURE;
	}
	size_t c = csfft(fs, as, num);
	printf("Прямое преобразование(%zu):\n", c); print_carray("(%f,%f)\n", as, num); puts("");
	c = cisfft(as, fs, num);
	printf("Обратное преобразование(%zu):\n", c); print_carray("(%f,%f)\n", fs, num); puts("");
	return EXIT_SUCCESS;
}
