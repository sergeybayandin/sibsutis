#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

size_t csfft(double complex* fs, double complex* as, size_t p1, size_t p2)
{
	size_t n = 0;
	double complex *_as = malloc(p1 * p2 * sizeof *_as);
	if (_as == NULL)
		return n;
	const double complex _2pii_p1 = -2.0 / p1 * M_PI * I;
	for (size_t i = 0; i < p1; ++i) {
		for (size_t j = 0; j < p2; ++j) {
			const size_t idx = i + p1 * j;
			_as[idx] = 0;
			for (size_t k = 0; k < p1; ++k, n += 4)
				_as[idx] += cexp(_2pii_p1 * k * i) * fs[j + p2 * k];
			_as[idx] /= p1; ++n;
		}
	}
	const double complex _2pii_p1p2 = -2.0 / (p1 * p2) * M_PI * I;
	for (size_t i = 0; i < p1; ++i) {
		for (size_t j = 0; j < p2; ++j) {
			const size_t idx = i + p1 * j;
			as[idx] = 0;
			for (size_t k = 0; k < p2; ++k, n += 4)
				as[idx] += cexp(_2pii_p1p2 * k * idx) * _as[i + p1 * k];
			as[idx] /= p2; ++n;
		}
	}
	free(_as);
	return n;
}

size_t cisfft(double complex* as, double complex* fs, size_t p1, size_t p2)
{
	size_t n = 0;
	double complex* _fs = malloc(p1 * p2 * sizeof *_fs);
	if (_fs == NULL)
		return n;
	const double complex _2pii_p1 = 2.0 / p1 * M_PI * I;
	for (size_t i = 0; i < p1; ++i) {
		for (size_t j = 0; j < p2; ++j) {
			const size_t idx = i + p1 * j;
			_fs[idx] = 0;
			for (size_t k = 0; k < p1; ++k, n += 4)
				_fs[idx] += cexp(_2pii_p1 * k * i) * as[j + p2 * k];
		}
	}
	const double complex _2pii_p1p2 = 2.0 / (p1 * p2) * M_PI * I;
	for (size_t i = 0; i < p1; ++i) {
		for (size_t j = 0; j < p2; ++j) {
			const size_t idx = i + p1 * j;
			fs[idx] = 0;
			for (size_t k = 0; k < p2; ++k, n += 4)
				fs[idx] += cexp(_2pii_p1p2 * k * idx) * _fs[i + p1 * k];
		}
	}
	free(_fs);
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
	input_array(fs, n);
	size_t p1;
	for (p1 = n > 1 ? sqrt(n) : 1; n % p1 != 0; --p1);
	size_t c = csfft(fs, as, p1, n / p1);
	printf("Прямое преобразование(%zu):\n", c); print_array(as, n); puts("");
	c = cisfft(as, fs, p1, n / p1);
	printf("Обратное преобразование(%zu):\n", c); print_array(fs, n); puts("");
	return EXIT_SUCCESS;
}
