#include "../include/multiplication.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	unsigned long long a, b, c;
	printf("Enter a, b: "); scanf("%llu %llu", &a, &b);
	struct operations_number on = fast_multiplication(a, b, &c);
	printf("T(n) = %llu + %llu\n", on.multiplications, on.additions);
	printf("%llu * %llu = %llu\n", a, b, c);
	return EXIT_SUCCESS;
}
