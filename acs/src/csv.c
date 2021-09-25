#include "../include/csv.h"
#include <string.h>
#include <stdio.h>

int get_csv_filename(char* buffer, size_t size)
{
	size_t i;
	ssize_t dot_pos = -1;
	int ch;
	for (i = 0; i < size && (ch = getchar()) != '\n' && ch != EOF; ++i) {
		if (ch == '.')
			dot_pos = i;
		buffer[i] = ch;
	}
	if (i < size)
		buffer[i] = '\0';
	else
		return -1;
	return dot_pos == -1 || strcmp(buffer + dot_pos, ".csv") != 0 ? -1 : 0 ;
}
