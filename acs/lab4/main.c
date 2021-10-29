#include "../include/blas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 512

int main(int argc, char* argv[])
{
	size_t n = DEFAULT_SIZE;
	if (argc > 1) {
		if (argc < 3) {
			fprintf(stderr, "Missing arguments\n");
			return EXIT_FAILURE;
		}
		if (argc > 3) {
			fprintf(stderr, "Too many arguments\n");
			return EXIT_FAILURE;
		}
		char* p;
		const char* cmd = argv[1];
		const char* arg = argv[2];
		if (strcmp("-n", cmd) == 0) {
			n = strtoull(arg, &p, 10);
			if (*p != '\0' || p == NULL) {
				fprintf(stderr, "Wrong value: %s\n", arg);
				return EXIT_FAILURE;
			}
		} else {
			fprintf(stderr, "Unknown command: %s\n", cmd);
			return EXIT_FAILURE;
		}
	}
	double* amtx, *bmtx, *cmtx;
	if ((amtx = malloc(n * n * sizeof *amtx)) == NULL ||
				(bmtx = malloc(n * n * sizeof *bmtx)) == NULL ||
					(cmtx = malloc(n * n * sizeof *cmtx)) == NULL) {
		fprintf(stderr, "Unable to allocate memory\n");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
