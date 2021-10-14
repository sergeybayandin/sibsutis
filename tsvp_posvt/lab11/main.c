#include "../include/multiplication.h"
#include <stdio.h>
#include <stdlib.h>

#define N 5
#define M 5

int main()
{
	size_t i;
	unsigned a[N], b[M], c[N + M];
	
	for (i = 0; i < N; ++i) 
		a[i] = 9;
	for (i = 0; i < M; ++i)
		b[i] = 9;
	
	struct operations_number on = stupid_column_multiplication(a, b, c, N, M, N + M);
	printf("T(n) = %zu + %zu = %zu\n", on.multiplications, on.additions, on.multiplications + on.additions);
	for (i = 0; i < N + M; ++i)
		printf("%u\t", c[i]);
	puts("");
	return EXIT_SUCCESS;
}
