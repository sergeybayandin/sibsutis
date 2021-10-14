#include "../include/iobigint.h"
#include <stdio.h>
#include <ctype.h>

#define NEGATIVE 1
#define POSITIVE 0

int get_bigint(unsigned short* bi, size_t num, int* negative)
{
	*negative = POSITIVE;
	int ch;
	size_t i = 0;
	if ((ch = getchar()) == '-')
		*negative = NEGATIVE;
	else if (isdigit(ch) != 0)
		bi[i++] = ch - '0';
	else if (ch != '+')
		return -1;
	for (; i < num && (ch = getchar()) != EOF && isspace(ch) == 0 && ch != '\n'; ++i) {
		if (isdigit(ch) != 0)
			bi[i] = ch - '0';
		else
			return -1;
	}
	return 0;
}

void print_bigint(const unsigned short* bi, size_t digits, int negative)
{
	if (negative == NEGATIVE)
		printf("-");
	for (size_t i = 0; i < digits; ++i)
		printf("%hu", bi[i]);
}
