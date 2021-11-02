#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 64
#define MAX_ARGS_COUNT 	5
#define MIN_ARGS_COUNT 	3
#define DEFAULT_N 	   	1024
#define DEFAULT_BS		 	2
#define FILENAME			 	"stats.txt"

int main(int argc, char* argv[])
{
	size_t n = DEFAULT_N, bs = DEFAULT_BS;
	if (argc > 1) {
		if (argc > MAX_ARGS_COUNT) {
			fprintf(stderr, "Too many arguments\n");
			return EXIT_FAILURE;
		}
		if (argc < MIN_ARGS_COUNT) {
			fprintf(stderr, "Missing arguments\n");
			return EXIT_FAILURE;
		}
		int i;
		char* p;
		for (i = 1; i < argc; i += 2) {
			if (i + 1 >= argc) {
				fprintf(stderr, "Missing arguments: %s\n", argv[i]);
				return EXIT_FAILURE;
			}
			if (strcmp("-n", argv[i]) == 0) {
				n = strtoull(argv[i + 1], &p, 10);
				if (*p != '\0' || p == NULL) {
					fprintf(stderr, "%s: wrong argument: %s\n", argv[i], argv[i + 1]);
					return EXIT_FAILURE;
				}
			} else if (strcmp("-bs", argv[i]) == 0) {
				bs = strtoull(argv[i + 1], &p, 10);
				if (*p != '\0' || p == NULL) {
					fprintf(stderr, "%s: wrong argument: %s\n", argv[i], argv[i + 1]);
					return EXIT_FAILURE;
				}
			} else {
				fprintf(stderr, "Unknown command: %s\n", argv[i]);
				return EXIT_FAILURE;
			}
		}
	}
	if ((n % bs) != 0) {
		fprintf(stderr, "n %% bs != 0\n");
		return EXIT_FAILURE;
	}
	char buffer[MAX_BUFFER_SIZE];
	sprintf(buffer, "perf stat -e cache-misses %s %zu", "./dgemm_o0", n);
	system(buffer);	
	sprintf(buffer, "perf stat -e cache-misses %s %zu", "./dgemm_o1", n);
	system(buffer);	
	sprintf(buffer, "perf stat -e cache-misses %s %zu %zu", "./dgemm_o2", n, bs);
	system(buffer);	
	sprintf(buffer, "perf stat -e cache-misses %s %zu", "./dgemm_o3", n);
	system(buffer);
	FILE* gp;
	if ((gp = popen("gnuplot -p", "w")) == NULL) {
		fprintf(stderr, "Failed to open gnuplot\n");
		return EXIT_FAILURE;
	}
	fprintf(gp, "set boxwidth 0.5\n");
	fprintf(gp, "set style fill solid\n");
	fprintf(gp, "plot '%s' using 1:3:xtic(2) with boxes title '-n = %zu, -bs = %zu'\n", FILENAME, n, bs);
	pclose(gp);
	remove(FILENAME);
	return EXIT_FAILURE;
}
