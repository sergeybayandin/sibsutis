#include "../include/blas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mmintrin.h> /* MMX */
#include <xmmintrin.h> /* SSE, нужен также mmintrin.h */
#include <emmintrin.h>

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
	size_t i, j;
	puts("a:");
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			scanf("%lf", &amtx[i * n + j]);	
	puts("b:");
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			scanf("%lf", &bmtx[i * n + j]);
	dgemm_o3_sse(amtx, bmtx, cmtx, n);
	puts("\nc:");
	for (i = 0; i < n; ++i, puts(""))
		for (j = 0; j < n; ++j)
			printf("%f\t", cmtx[i * n + j]);
	return EXIT_SUCCESS;
}
