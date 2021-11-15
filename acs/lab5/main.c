#include <stdio.h>
#include "../include/blas.h"
#include <stdlib.h>
#include <time.h>

#define N 2000

int main()
{
	double *a, *b, *c;
	a = malloc(N * sizeof *a);
	b = malloc(N * sizeof *b);
	c = malloc(N * sizeof *c);
	clock_t start = clock();
	dgemm_o0(a, b, c, 1000);
	clock_t finish = clock();
	printf("t1=%d\n", finish - start);
	/*start = clock();
	dgemm_o0(a, b, c, 1500);
	finish = clock();
	printf("t2=%d\n", finish - start);
	start = clock();
	dgemm_o0(a, b, c, 2000);
	finish = clock();
	printf("t3=%d\n", finish - start);*/
	return 0;
}
