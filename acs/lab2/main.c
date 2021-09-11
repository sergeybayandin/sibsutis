#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define OPERAND_TYPE 	  double
#define OPERAND_TYPE_S "double"
#define TIMER_S        "clock"
#define MIN_N_LAUNCH    10
#define UPPER           2000
#define DOWN            1000

double* buf;
FILE* csvtab;
char processor_model[50], filename[20], optimizations[20];
size_t nlaunch;

void test(const char* task_name, OPERAND_TYPE (*task)(OPERAND_TYPE))
{
	double average = 0.0;
	for (size_t i = 0; i < nlaunch; ++i) {
		const OPERAND_TYPE operand = rand() % UPPER - DOWN;
		const clock_t start = clock();
		task(operand);
		const clock_t finish = clock();
		average += (buf[i] = (double)((finish - start)) / CLOCKS_PER_SEC);
	}
	average /= nlaunch;
	for (size_t i = 0; i < nlaunch; ++i) {
		fprintf(csvtab, "%s;%s;%s;%s;%u;%s;%f;%u;%f;%f;%f;%f\n", processor_model, 
														 									 				 			 task_name, 
														 									 				 			 OPERAND_TYPE_S,
														 									 				 			 optimizations, 
														 									 							 nlaunch,
														 									 				 			 TIMER_S,
																							 				 			 buf[i], 
														 									 				 			 i + 1,
					 																		 				 			 average,
																							 				 			 0.0,
																							 				 			 0.0,
																							 				 			 0.0);
	}
}

int main(int argc, char* argv[])
{
	if (argc > 1)	{
		char* end;
		nlaunch = strtoul(argv[1], &end, 10);
		if (*end) {
			fprintf(stderr, "Невозможно конвертировать %s в число\n", argv[1]);
			return EXIT_FAILURE;
		}
	} else {
		nlaunch = MIN_N_LAUNCH;
	}

	FILE* grep = popen("cat makefile | grep -oP \'FLAGS=\\s*\\K.+\'", "r");
	if (grep == NULL) {
		fprintf(stderr, "Невозможно получить ключи оптимизации или отсутствует makefile\n");
		return EXIT_FAILURE;
	}
	fgets(optimizations, sizeof(optimizations), grep);
	pclose(grep);
	if (*optimizations != '\0')
		*strchr(optimizations, '\n') = '\0';
	else
		strcpy(optimizations, "None");

	buf = malloc(sizeof(*buf) * nlaunch);
	if (buf == NULL) {
		fprintf(stderr, "Невозможно выделить память под буффер\n");
		return EXIT_SUCCESS;
	}

	grep = popen("lscpu | grep -oP \'Имя модели:\\s*\\K.+\'", "r");
	if (grep == NULL) {
		perror("Невозможно запустить lscpu или grep");
		return EXIT_FAILURE;
	}
	fgets(processor_model, sizeof(processor_model), grep);
	*strchr(processor_model, '\n') = '\0';
	pclose(grep);
	
	printf("Введите имя .csv файла: ");
	const size_t n = sizeof(filename) - 1;
	ssize_t i = 0, dot_idx = -1;
	for (int c; (c = getchar()) != EOF && i < n; ++i) {
		if (c == '.')
			dot_idx = i;
		if (c == '\n')
			break;
		filename[i] = c;
	}
	filename[i] = '\0';
	if (strcmp(filename + dot_idx + 1, "csv") != 0) {
		perror("Расширение должно быть .csv");
		return EXIT_FAILURE;
	}
	
	csvtab = fopen(filename, "w");
	if (csvtab == NULL) {
		fprintf(stderr, "Невозможно открыть %s\n", filename);
		return EXIT_FAILURE;
	}
	
	srand(time(NULL));

	test("sin", sin);
	test("cos", cos);
	test("tan", tan);	
	
	return EXIT_SUCCESS;
}
