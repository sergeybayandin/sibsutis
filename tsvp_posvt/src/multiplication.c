#include "../include/multiplication.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 10
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void __carry(unsigned* a, size_t digits)
{
	size_t i;
	for (i = digits - 1; i > 0; --i) {
		if (a[i] >= BASE) {
			a[i - 1] += a[i] / BASE;
			a[i] %= BASE;
		}
	}
}

void __polynomial_addition(const unsigned* a, const unsigned* b, 
	unsigned* c, size_t bnum, size_t acnum)
{
	size_t i;
	memcpy(c, a, acnum * sizeof *c);
	--acnum; --bnum;
	for (i = 0; i <= bnum; ++i)
		c[acnum - i] += b[bnum - i];
}

void __polynomial_subtraction_assignment(unsigned* a, const unsigned* b, size_t anum, size_t bnum)
{
	size_t i;
	--anum; --bnum;
	for (i = 0; i <= bnum; ++i)
		a[anum - i] -= b[bnum - i];
}

void __polynomial_addition_assignment(unsigned* a, const unsigned* b, size_t anum, size_t bnum)
{
	size_t i;
	--anum; --bnum;
	for (i = 0; i <= bnum; ++i)
		a[anum - i] += b[bnum - i];
}

void __polynomial_fast_multiplication(const unsigned* a, const unsigned* b, 
	unsigned* c, size_t abnum, size_t cnum, struct operations_number* on)
{
	if (abnum == 1) {
		*c = *a * *b;
		++on->multiplications;
		return;
	}
	size_t rnum = abnum / 2, lnum = rnum + abnum % 2, 
		alblnum = 2 * lnum - 1, arbrnum = 2 * rnum - 1;
	unsigned* albl, *arbr, *alar, *blbr, *alarblbr;
	if ((albl = calloc(alblnum, sizeof *albl)) == NULL)
		return;
	__polynomial_fast_multiplication(a, b, albl, lnum, alblnum, on);
	if ((arbr = calloc(arbrnum, sizeof *arbr)) == NULL) {
		free(albl);
		return;
	}
	__polynomial_fast_multiplication(a + lnum, b + lnum, arbr, rnum, arbrnum, on);
	if ((alar = malloc(lnum * sizeof *alar)) == NULL) {
		free(albl); free(arbr);
		return;
	}
	__polynomial_addition(a, a + lnum, alar, rnum, lnum);
	if ((blbr = malloc(lnum * sizeof *blbr)) == NULL) {
		free(albl); free(arbr); free(alar);
		return;
	}
	__polynomial_addition(b, b + lnum, blbr, rnum, lnum);
	if ((alarblbr = calloc(alblnum, sizeof *alarblbr)) == NULL) {
		free(albl); free(arbr); free(alar); free(blbr);
		return;
	}
	__polynomial_fast_multiplication(alar, blbr, alarblbr, lnum, alblnum, on);
	__polynomial_subtraction_assignment(alarblbr, albl, alblnum, alblnum);
	__polynomial_subtraction_assignment(alarblbr, arbr, alblnum, arbrnum);
	abnum /= 2;
	__polynomial_addition_assignment(c, albl, cnum - 2 * abnum, alblnum);
	__polynomial_addition_assignment(c, alarblbr, cnum - abnum, alblnum);
	__polynomial_addition_assignment(c, arbr, cnum, arbrnum);
	on->additions += 4;
	free(albl); free(arbr); free(alar); free(blbr); free(alarblbr);
}

void __stupid_polynomial_multiplication(const unsigned* a, const unsigned* b, unsigned* c, 
	size_t anum, size_t bnum, size_t cnum, struct operations_number* on)
{
	size_t i, j;
	for (i = 0; i < bnum; ++i, ++on->multiplications)
		c[cnum - i - 1] = a[anum - 1] * b[bnum - i - 1];
	memset(c, 0x00, (cnum - i) * sizeof *c);
	--anum; --bnum; --cnum;
	for (i = 1; i <= anum; ++i, ++on->additions)
		for (j = 0; j <= bnum; ++j, ++on->multiplications)
			c[cnum - i - j] += a[anum - i] * b[bnum - j];
}

struct operations_number
stupid_column_multiplication(const unsigned* a, const unsigned* b, unsigned* c, size_t adigits, size_t bdigits, size_t cdigits)
{
	size_t i, j;
	struct operations_number on = { 0, 0 };
	__stupid_polynomial_multiplication(a, b, c + 1, adigits, bdigits, cdigits - 1, &on);
	__carry(c, cdigits);
	return on;
}

struct operations_number
fast_multiplication(const unsigned* a, const unsigned* b, unsigned* c, size_t abdigits, size_t cdigits)
{
	struct operations_number on = { 0, 0 };	
	memset(c, 0x00, cdigits * sizeof *c);
	__polynomial_fast_multiplication(a, b, c + 1, abdigits, cdigits - 1, &on);
	__carry(c, cdigits);
	return on;
}

void very_fast_multiplication(const unsigned* a, const unsigned* b, unsigned* c,
	size_t abdigits, size_t cdigits)
{
	
}
