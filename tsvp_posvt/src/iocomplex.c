#include "../../include/tsvp_posvt/iocomplex.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 32

int get_carray(carray_t carr, size_t num)
{
	char buffer[MAX_BUFFER_SIZE];
	int ch;
	size_t j;
	double real, imag;
	char* end;
	for (size_t i = 0; i < num && (ch = getchar()) != EOF && ch != '\n'; ++i) {
		switch (ch) {
			case ' ' : {
				while (isspace(getchar()) != 0);
				break;
			} 
			case '(' : break;
			default : return -1;
		}
		for (j = 0; (ch = getchar()) != EOF && ch != '\n' && ch != ','; ++j) {
			if (j > MAX_BUFFER_SIZE)
				return -1;
			buffer[j] = ch;
		}	
		if (ch != ',')
			return -1;
		buffer[j] = '\0';
		real = strtod(buffer, &end);
		if (*end != '\0')
			return -1;
		for (j = 0; (ch = getchar()) != EOF && ch != '\n' && ch != ')'; ++j) {
			if (j > MAX_BUFFER_SIZE)
				return -1;
			buffer[j] = ch;
		}
		if (ch != ')')
			return -1;
		buffer[j] = '\0';
		imag = strtod(buffer, &end);
		if (*end != '\0')
			return -1;
		carr[i] = real + I * imag;
	}
	return 0;
}

void print_carray(const char* fmt, const carray_t carr, size_t num)
{
	for (size_t i = 0; i < num; ++i)
		printf(fmt, creal(carr[i]), cimag(carr[i]));
}
