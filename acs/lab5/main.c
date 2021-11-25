#include <stdio.h>
#include "../include/blas.h"
#include <stdlib.h>
#include <time.h>

#define N 2000

int main()
{
	double *a, *b, *c;
	a = malloc(N * N * sizeof *a);
	b = malloc(N * N * sizeof *b);
	c = malloc(N * N * sizeof *c);
	clock_t start = clock();
	dgemm_o0(a, b, c, 1000);
	clock_t finish = clock();
	printf("t1=%f\n", ((double)finish - start) / CLOCKS_PER_SEC);
	start = clock();
	dgemm_o0(a, b, c, 1500);
	finish = clock();
	printf("t2=%f\n", ((double)finish - start) / CLOCKS_PER_SEC);
	start = clock();
	dgemm_o0(a, b, c, N);
	finish = clock();
	printf("t3=%f\n", ((double)finish - start) / CLOCKS_PER_SEC);
	return 0;
}
