#include "../include/multiplication.h"
#include "../include/iobigint.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t anum, bnum, cnum;
	printf("Enter n, m: "); scanf("%zu %zu", &anum, &bnum); getchar();
	cnum = anum + bnum;
	unsigned* a, *b, *c;
	if ((a = malloc(anum * sizeof *a)) == NULL ||
				(b = malloc(bnum * sizeof *b)) == NULL ||
					(c = malloc(cnum * sizeof *c)) == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return EXIT_FAILURE;
	}
	int anegative = 0, bnegative = 0;
	if (get_bigint(a, anum, &anegative) != 0) {
		fprintf(stderr, "Input failed\n");
		return EXIT_FAILURE;
	}
	getchar();
	if (get_bigint(b, bnum, &bnegative) != 0) {
		fprintf(stderr, "Input failed\n");
		return EXIT_FAILURE;
	}
 	struct operations_number on = stupid_column_multiplication(a, b, c, anum, bnum, cnum);
	printf("T(n) = %zu + %zu = %zu\n", on.multiplications, on.additions, on.multiplications + on.additions);
	print_bigint(a, anum, anegative); 
	printf(" * "); 
	print_bigint(b, bnum, bnegative);
	printf(" = ");
	if (*c != 0)
		print_bigint(c, cnum, anegative + bnegative);
	else
		print_bigint(c + 1, cnum - 1, anegative + bnegative);
	puts("");
	return EXIT_SUCCESS;
}
