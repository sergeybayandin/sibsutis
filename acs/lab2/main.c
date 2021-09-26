#include "../include/test_cpu/cpuinfo.h"
#include "../include/test_cpu/test_cpu.h"
#include "../include/csv.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MAX_BUFFER_SIZE 64
#define MIN_N_LAUNCH    10
#define FORMAT 					"%s,%s,%s,%s,%d,rdtsc,%.8f,%zu,%.8f,%.8f,%.8f%%,%.8f\n"

void write_csv(FILE*, const char*, const char*, const char*, const char*, struct statistics*);

int main(int argc, char* argv[])
{
	int typeid = -1;
	struct statistics stats = {MIN_N_LAUNCH, NULL, 0.0, 0.0, 0.0, 0.0};
	char operand_type[MAX_BUFFER_SIZE]  = "double", 
			 optimizations[MAX_BUFFER_SIZE] = "None";
	if (argc > 1) {
		char* end;
		size_t pos = 0;
		while (--argc > 0) {
			if (typeid == -1 && strcmp(argv[argc], "float") == 0) {
				typeid = FLOAT;
				strcpy(operand_type, "float");
			} else if (typeid == -1 && strcmp(argv[argc], "long double") == 0) {
				typeid = LONG_DOUBLE;
				strcpy(operand_type, "long double");
			} else if (typeid == -1 && strcmp(argv[argc], "double") == 0) { 
				typeid = DOUBLE;
				strcpy(operand_type, "double");
			} else if (strncmp(argv[argc], "-O", 2) == 0) {
				int size = strlen(argv[argc]);
				for (int i = 2; i < size; ++i) {
					if (isdigit(argv[argc][i]) == 0) {
						fprintf(stderr, "Неверный аргумент: %s\n", argv[argc]);
						return EXIT_FAILURE;
					}
				}
				memcpy(optimizations + pos, argv[argc], size);
				pos += size + 1;
				optimizations[pos - 1] = ' ';
				if (pos > MAX_BUFFER_SIZE) {
					fprintf(stderr, "Слишком много ключей оптимизации\n");
					return EXIT_FAILURE;
				}
			} else {
				const size_t num = strtoul(argv[argc], &end, 10);
				if (*end != '\0') {
					fprintf(stderr, "Неверный аргумент: %s\n", argv[argc]);
					return EXIT_FAILURE;
				}
				if (stats.nlaunch == MIN_N_LAUNCH && num > MIN_N_LAUNCH)
					stats.nlaunch = num;
			}
		}
		if (pos > 0)
			optimizations[pos - 1] = '\0';
	}		
	if (typeid == -1)
		typeid = DOUBLE;
	if ((stats.launch_time = malloc(stats.nlaunch * sizeof *stats.launch_time)) == NULL) {
		fprintf(stderr, "Не удалось выделить память\n");
		return EXIT_FAILURE;
	}	
	char processor_model_name[MAX_BUFFER_SIZE];
	if (get_processor_model_name(processor_model_name, sizeof(processor_model_name)) == -1) {
		fprintf(stderr, "Не удалось получить модель процессора\n");
		return EXIT_FAILURE;
	}
	double max_clock_frequency;
	if (get_max_clock_frequency(&max_clock_frequency) == -1) {
		fprintf(stderr, "Не удалось получить максимальную тактовую частоту процессора\n");
		return EXIT_FAILURE;
	}
	char csv_filename[MAX_BUFFER_SIZE];
	printf("ВВедите имя .csv файла: ");
	if (get_csv_filename(csv_filename, sizeof(csv_filename)) == -1) {
		fprintf(stderr, "Некорректное имя: %s\n", csv_filename);
		return EXIT_FAILURE;
	}
	FILE* csv_file;
	if ((csv_file = fopen(csv_filename, "w")) == NULL) {
		fprintf(stderr, "Не удалось открыть %s\n", csv_filename);
		return EXIT_FAILURE;
	}
	test_cpu(&stats, max_clock_frequency, typeid, SIN); 
	write_csv(csv_file, processor_model_name, operand_type, "sin", optimizations, &stats);
	test_cpu(&stats, max_clock_frequency, typeid, COS);
	write_csv(csv_file, processor_model_name, operand_type, "cos", optimizations, &stats);
	test_cpu(&stats, max_clock_frequency, typeid, TAN);
	write_csv(csv_file, processor_model_name, operand_type, "tan", optimizations, &stats);
	fclose(csv_file);
	FILE* temp, *pipe;
	if ((temp = fopen("temp.txt", "w")) == NULL) {
		fprintf(stderr, "Не удалось создать временный файл\n");
		return EXIT_FAILURE;
	}
	char pipe_command[MAX_BUFFER_SIZE]; sprintf(pipe_command, "cat %s | gawk -F \',\' \'{printf $2\"\\t\"$12\"\\n\"}\' | uniq", csv_filename);
	if ((pipe = popen(pipe_command, "r")) == NULL) {
		fprintf(stderr, "Не удалось открыть cat, gawk или uniq\n");
		return EXIT_FAILURE;
	}
	for (int i = 0; feof(pipe) == 0; ++i)
		if (fgets(pipe_command, MAX_BUFFER_SIZE, pipe) != NULL)
			fprintf(temp, "%d\t%s\n", i, pipe_command);
	pclose(pipe);
	fclose(temp);
	FILE* gp;
	if ((gp = popen("gnuplot -p", "w")) == NULL) {
		fprintf(stderr, "Не удалось открыть gnuplot\n");
		return EXIT_FAILURE;
	}
	fprintf(gp, "set boxwidth 0.5\n");
	fprintf(gp, "set style fill solid\n");
	fprintf(gp, "plot 'temp.txt' using 1:3:xtic(2) with boxes title 'operand type: %s'\n", operand_type);
	pclose(gp);
	if (remove("temp.txt") != 0) {
		fprintf(stderr, "Не удалось удалить временный файл\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void write_csv(FILE* csv_file, const char* processor_model_name, 
	const char* operand_type, const char* task, const char* optimizations, struct statistics* stats)
{
	for (size_t i = 0; i < stats->nlaunch; ++i)
		fprintf(csv_file, FORMAT, processor_model_name,
												 			task,
												 			operand_type,
												 			optimizations,
												 			1,
												 			stats->launch_time[i],
												 			i + 1,
												 			stats->average_time,
												 			stats->absolute_error,
												 			stats->relative_error,
												 			stats->task_perfomance
					 );
}
