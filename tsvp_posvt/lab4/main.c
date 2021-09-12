#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <ctype.h>
#include <math.h>

size_t cdft(double complex* fs, double complex* as, size_t num)
{
	size_t n = 0;
	const double complex _2pii_n = -2.0 * M_PI / num * I;
	for (size_t i = 0; i < num; ++i) {
		as[i] = 0;
		for (size_t j = 0; j < num; ++j, n += 4)
			as[i] += cexp(_2pii_n * i * j) * fs[j];
		as[i] /= num; ++n;
	}
	return n;
}

size_t cidft(double complex* as, double complex* fs, size_t num)
{
	size_t n = 0;
	const double complex _2pii_n = 2.0 * M_PI / num * I;
	for (size_t i = 0; i < num; ++i) {
		fs[i] = 0;
		for (size_t j = 0; j < num; ++j, n += 4)
			fs[i] += cexp(_2pii_n * i * j) * as[j];
	}
	return n;
}

void input_array(double complex* arr, size_t n)
{
	char buffer[20]; 
	double real, imag;
	for (size_t i = 0; i < n; ++i) {
		while (isspace(getchar()) != 0);
		size_t j = 0;
		for (int c; (c = getchar()) != ',';)
			if (isspace(c) == 0)
				buffer[j++] = c;
		buffer[j] = '\0';
		real = atof(buffer);
		j = 0;
		for (int c; (c = getchar()) != ')';)
			if (isspace(c) == 0)
				buffer[j++] = c;
		buffer[j] = '\0';
		imag = atof(buffer);
		arr[i] = real + imag * I;
	}
}

void print_array(double complex* arr, size_t num)
{
	for (size_t i = 0; i < num; ++i)
		printf("(%f,%f)\n", creal(arr[i]), cimag(arr[i]));
}

int main()
{
	size_t n; 
	printf("Введите размер массива: "); scanf("%zu", &n);
	double complex* fs = malloc(n * sizeof *fs);
	double complex* as = malloc(n * sizeof *as);
	if (fs == NULL || as == NULL) {
		fprintf(stderr, "Не удалось выделить память под массивы\n");
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < n; ++i)
		fs[i] = i + I * 0;
	size_t c = cdft(fs, as, n);
	printf("Прямое ДПФ(%zu):\n", c); print_array(as, n); puts("");
	c = cidft(as, fs, n);
	printf("Обратное ДПФ(%zu):\n", c); print_array(fs, n); puts("");
	return EXIT_SUCCESS;
}
