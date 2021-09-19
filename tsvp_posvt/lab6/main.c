#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef double complex complex_t;
typedef complex_t*		 array_t;

void cfft_impl(array_t fs, array_t as, array_t ws, size_t num, size_t* n)
{
	if (num < 2)
		return;

	const size_t mid = num / 2;
	array_t a0 = malloc(mid * sizeof *a0);
	array_t a1 = malloc(mid * sizeof *a1);

	if (a0 == NULL || a1 == NULL)
		return;

	for (size_t i = 0, j = 0; i < mid; ++i, ++j, *n += 2) {
		a0[j] = fs[2 * i];
		a1[j] = fs[2 * i + 1];
	}
	
	cfft_impl(a0, a0, ws, mid, n);
	cfft_impl(a1, a1, ws, mid, n);
	
	for (size_t i = 0; i < mid; ++i, *n += 4) {
		as[i] = a0[i] + ws[i] * a1[i];
		as[mid + i] = a0[i] - ws[i] * a1[i];
	}	

	free(a0); free(a1);
}

size_t cfft(array_t fs, array_t as, size_t num)
{
	size_t n = 0;
	const size_t mid = num / 2;
	array_t ws = malloc(mid * sizeof *ws);
	if (ws == NULL)
		return n;
	const complex_t w = cexp(-2.0 / num * I * M_PI);
	ws[0] = 1;
	for (size_t i = 1; i < mid; ++i)
		ws[i] = ws[i - 1] * w;
	cfft_impl(fs, as, ws, num, &n);
	for (size_t i = 0; i < num; ++i)
		as[i] /= num;
	free(ws);
	return n;	
}

size_t cifft(array_t as, array_t fs, size_t num)
{
	size_t n = 0;
	const size_t mid = num / 2;
	array_t ws = malloc(mid * sizeof *ws);
	if (ws == NULL)
		return n;
	const complex_t w = cexp(2.0 / num * I * M_PI);
	ws[0] = 1;
	for (size_t i = 1; i < mid; ++i)
		ws[i] = ws[i - 1] * w;
	cfft_impl(as, fs, ws, num, &n);
	free(ws);
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

void print_array(array_t arr, size_t num)
{
	for (size_t i = 0; i < num; ++i)
		printf("(%f,%f)\n", creal(arr[i]), cimag(arr[i]));
}

int main()
{
	size_t num;
	printf("Введите размер массива: "); scanf("%zu", &num);
	array_t fs = malloc(num * sizeof *fs);
	array_t as = malloc(num * sizeof *as);
	if (fs == NULL || as == NULL) {
		fprintf(stderr, "Не удалось выделить память под массивы\n");
		return EXIT_FAILURE;
	}
	input_array(fs, num);
	size_t n = cfft(fs, as, num);
	printf("Прямое преобразование(%zu):\n", n); print_array(as, num); puts("");
	n = cifft(as, fs, num);
	printf("Обратное преобразование(%zu):\n", n); print_array(fs, num); puts("");
	return EXIT_SUCCESS;
}
