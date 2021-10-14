#include "../include/multiplication.h"
#include "../include/iobigint.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t anum, bnum, cnum, maxabnum;
	printf("Enter n, m: "); scanf("%zu %zu", &anum, &bnum); getchar();
	cnum = anum + bnum;
	maxabnum = anum > bnum ? anum : bnum;
	unsigned* a, *b, *c;
	if ((a = calloc(maxabnum, sizeof* a)) == NULL ||
				(b = calloc(maxabnum, sizeof* b)) == NULL ||
					(c = malloc(cnum * sizeof *c)) == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return EXIT_FAILURE;
	}
	int anegative, bnegative;
	if (get_bigint(a + (maxabnum - anum), anum, &anegative) != 0) {
		fprintf(stderr, "Input failed\n");
		return EXIT_FAILURE;
	}	
	getchar();
	if (get_bigint(b + (maxabnum - bnum), bnum, &bnegative) != 0) {
		fprintf(stderr, "Input failed\n");
		return EXIT_FAILURE;
	}
	struct operations_number on = fast_multiplication(a, b, c, maxabnum, cnum);
	printf("T(n) = %zu + %zu = %zu\n", on.multiplications, on.additions, on.multiplications + on.additions);
	print_bigint(a + (maxabnum - anum), anum, anegative);
	printf(" * ");
	print_bigint(b + (maxabnum - bnum), bnum, bnegative);
	printf(" = ");
	if (*c != 0)
		print_bigint(c, cnum, anegative + bnegative);
	else
		print_bigint(c + 1, cnum - 1, anegative + bnegative);
	puts("");
	return EXIT_SUCCESS;
}
