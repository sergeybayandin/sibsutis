#include "../include/test_memory/memoryinfo.h"
#include "../include/test_memory/test_memory.h"
#include "../include/test_cpu/cpuinfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MIN_N_LAUNCH 	   10
#define MIN_BLOCK_SIZE   512
#define MIN_BUFFER_SIZE  sizeof(int)
#define KB_BYTES_COUNT   1024
#define MB_BYTES_COUNT   KB_BYTES_COUNT * 1024
#define GB_BYTES_COUNT   MB_BYTES_COUNT * 1024
#define CSVFILENAME		   "statistics.csv"
#define FORMAT					 "%s,%zu,%zu,%zu,rdtsc,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n"

void write_csv(FILE*, size_t, size_t, struct memory_device_info*);

int main(int argc, char* argv[])
{
	int show_diagram = 0;
	size_t nlaunch = MIN_N_LAUNCH, block_size = MIN_BLOCK_SIZE, buffer_size = MIN_BUFFER_SIZE;
	int ram = 0, hdd = 0, ssd = 0, flash = 0;
	if (argc > 1) {
		char* p, *p1;
		size_t num;
		for (int i = 1; i < argc; i += 2) {
			if (i + 1 >= argc && strcmp(argv[i], "-s") != 0 && strcmp(argv[i], "--show-diagram") != 0) {
				fprintf(stderr, "%s: mising arguments\n", argv[i]);
				return EXIT_FAILURE;
			}
			if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--memory-type") == 0) {
				p = argv[i + 1];
				int j, k;
				const int len = strlen(p);
				for (j = 0, k = 0; j <= len; ++j) {
					if (p[j] != '|' && p[j] != '\0')
						continue;
					p[j] = '\0';
					for (int c = k; p[c] != '\0'; ++c)
						p[c] = tolower(p[c]);
					if (ram == 0 && strcmp(p + k, "ram") == 0) {
						ram = RAM;
					} else if (hdd == 0 && strcmp(p + k, "hdd") == 0) {
						hdd = HDD;
					} else if (ssd == 0 && strcmp(p + k, "ssd") == 0) {
						ssd = SSD;
					} else if (flash == 0 && strcmp(p + k, "flash") == 0) {
						flash = FLASH;
					} else {
						if (j < len)
							p[j] = '|';
						fprintf(stderr, "%s: wrong argument\n", p);
						return EXIT_FAILURE;
					}
					p[j] = '|';
					k = j + 1;
				}
			} else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--block-size") == 0) {
				block_size = strtoul(argv[i + 1], &p, 10);
				if (*p != '\0') {
					const int len = strlen(p);
					for (int j = 0; j < len; ++j)
						p[j] = tolower(p[j]);
					if (strcmp(p, "kb") == 0) {
						block_size *= KB_BYTES_COUNT;
					} else if (strcmp(p, "mb") == 0) {
						block_size *= MB_BYTES_COUNT;
					} else if (strcmp(p, "gb") == 0) {
						block_size *= GB_BYTES_COUNT;
					} else if (strcmp(p, "b") == 0) {
						block_size *= 1;
					} else {
						fprintf(stderr, "%s: missing argument\n", argv[i + 1]);
						return EXIT_FAILURE;
					}
				}
			} else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--launch-count") == 0) {
				num = strtoul(argv[i + 1], &p, 10);
				if (*p != '\0') {
					fprintf(stderr, "%s: wrong argument\n", argv[i + 1]);
					return EXIT_FAILURE;
				}
				if (nlaunch == MIN_N_LAUNCH && num > MIN_N_LAUNCH)
					nlaunch = num;
			} else if (strcmp(argv[i], "-bs") == 0 || strcmp(argv[i], "--buffer-size") == 0) {
				num = strtoul(argv[i + 1], &p, 10);
				if (*p != '\0') {
					fprintf(stderr, "%s: wrong argument\n", argv[i + 1]);
					return EXIT_FAILURE;
				}
				if (buffer_size == MIN_BUFFER_SIZE && num > MIN_BUFFER_SIZE)
					buffer_size = num;
			} else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--show-diagram") == 0) {
				show_diagram = 1; --i;
			} else {
				fprintf(stderr, "%s: unknown command\n", argv[i]);
				return EXIT_FAILURE;
			}
		}
	}
	if (block_size % buffer_size != 0) {
		fprintf(stderr, "");
		return EXIT_FAILURE;
	}
	if (ram == 0 && hdd == 0 && ssd == 0 && flash == 0)
		ram = RAM;
	void* block;
	if ((block = malloc(block_size)) == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return EXIT_FAILURE;
	}
	struct memory_devices_info mds = get_memory_devices_info(ram, hdd, ssd, flash);
	if (mds.mdis == NULL) {
		fprintf(stderr, "Failed to get information about memory devices\n");
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		if (mds.mdis[i].type != RAM) {
			SMDI_CAST(mds.mdis + i)->stats.nlaunch = nlaunch;
			if ((SMDI_CAST(mds.mdis + i)->stats.w_launch_time = malloc(nlaunch * sizeof *SMDI_CAST(mds.mdis + i)->stats.w_launch_time)) == NULL) {
				fprintf(stderr, "Cannot allocate memory\n");
				return EXIT_FAILURE;
			}	
			if ((SMDI_CAST(mds.mdis + i)->stats.r_launch_time = malloc(nlaunch * sizeof *SMDI_CAST(mds.mdis + i)->stats.r_launch_time)) == NULL) {
				fprintf(stderr, "Cannot allocate memory\n");
				return EXIT_FAILURE;
			}
		} else {
			RMDI_CAST(mds.mdis + i)->stats.nlaunch = nlaunch;			
			if ((RMDI_CAST(mds.mdis + i)->stats.w_launch_time = malloc(nlaunch * sizeof *RMDI_CAST(mds.mdis + i)->stats.w_launch_time)) == NULL) {
				fprintf(stderr, "Cannot allocate memory\n");
				return EXIT_FAILURE;
			}			
			if ((RMDI_CAST(mds.mdis + i)->stats.r_launch_time = malloc(nlaunch * sizeof *RMDI_CAST(mds.mdis + i)->stats.r_launch_time)) == NULL) {
				fprintf(stderr, "Cannot allocate memory\n");
				return EXIT_FAILURE;
			}
		}
	}
	double max_clock_frequency;
	if (get_max_clock_frequency(&max_clock_frequency) != 0) {
		fprintf(stderr, "Failed to get max clock frequency\n");
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i)
		test_memory(block, block_size, buffer_size, max_clock_frequency, mds.mdis + i);
	FILE* csv_file;
	if ((csv_file = fopen(CSVFILENAME, "a+")) == NULL) {
		fprintf(stderr, "Cannot open csv file\n");
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i)
		write_csv(csv_file, block_size, buffer_size, mds.mdis + i);
	fclose(csv_file);
	if (show_diagram == 0)
		return EXIT_SUCCESS;
	FILE* gp, *pipe, *w_temp_file, *r_temp_file;
	if ((gp = popen("gnuplot -p", "w")) == NULL) {
		fprintf(stderr, "Cannot open gnuplot\n");
		return EXIT_FAILURE;
	}
	if ((w_temp_file = fopen("wtemp.txt", "w")) == NULL) {
		fprintf(stderr, "Cannot open temp file\n");
		return EXIT_FAILURE;
	}		
	if ((r_temp_file = fopen("rtemp.txt", "w")) == NULL) {
		fprintf(stderr, "Cannot open temp file\n");
		return EXIT_FAILURE;
	}
	char buffer[64]; sprintf(buffer, "cat %s | gawk -F \',\' \'{print $8}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(w_temp_file, "%zu\t\"%s\"\t%s", i, model_name, buffer);
	}
	pclose(pipe);
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $13}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(r_temp_file, "%zu\t\"%s\"\t%s", i + mds.count, model_name, buffer);
	}
	pclose(pipe);
	fclose(w_temp_file);
	fclose(r_temp_file);
	fprintf(gp, "set boxwidth 0.5\n");
	fprintf(gp, "set style fill solid\n");
	fprintf(gp, "plot 'wtemp.txt' using 1:3:xtic(2) with boxes title 'write(%zu)', 'rtemp.txt' using 1:3:xtic(2) with boxes title 'read(%zu)'\n", block_size, block_size);
	pclose(gp);

	if ((gp = popen("gnuplot -p", "w")) == NULL) {
		fprintf(stderr, "Cannot open gnuplot\n");
		return EXIT_FAILURE;
	}
	if ((w_temp_file = fopen("wtemp.txt", "w")) == NULL) {
		fprintf(stderr, "Cannot open temp file\n");
		return EXIT_FAILURE;
	}		
	if ((r_temp_file = fopen("rtemp.txt", "w")) == NULL) {
		fprintf(stderr, "Cannot open temp file\n");
		return EXIT_FAILURE;
	}
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $9}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(w_temp_file, "%zu\t\"%s(wabs)\"\t%s", i, model_name, buffer);
	}
	pclose(pipe);	
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $10}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(w_temp_file, "%zu\t\"%s(wrel)\"\t%s", i + mds.count, model_name, buffer);
	}
	pclose(pipe);	
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $14}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(r_temp_file, "%zu\t\"%s(rabs)\"\t%s", i + 2 * mds.count, model_name, buffer);
	}
	pclose(pipe);
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $15}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(r_temp_file, "%zu\t\"%s(rrel)\"\t%s", i + 3 * mds.count, model_name, buffer);
	}
	pclose(pipe);
	fclose(w_temp_file);
	fclose(r_temp_file);
	fprintf(gp, "set boxwidth 0.5\n");
	fprintf(gp, "set style fill solid\n");
	fprintf(gp, "plot 'wtemp.txt' using 1:3:xtic(2) with boxes title 'write(%zu)', 'rtemp.txt' using 1:3:xtic(2) with boxes title 'read(%zu)'\n", block_size, block_size);
	pclose(gp);

	if ((gp = popen("gnuplot -p", "w")) == NULL) {
		fprintf(stderr, "Cannot open gnuplot\n");
		return EXIT_FAILURE;
	}
	if ((w_temp_file = fopen("wtemp.txt", "w")) == NULL) {
		fprintf(stderr, "Cannot open temp file\n");
		return EXIT_FAILURE;
	}		
	if ((r_temp_file = fopen("rtemp.txt", "w")) == NULL) {
		fprintf(stderr, "Cannot open temp file\n");
		return EXIT_FAILURE;
	}
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $9}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(w_temp_file, "%zu\t\"%s(wabs)\"\t%s", i, model_name, buffer);
	}
	pclose(pipe);	
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $10}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(w_temp_file, "%zu\t\"%s(wrel)\"\t%s", i + mds.count, model_name, buffer);
	}
	pclose(pipe);	
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $14}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(r_temp_file, "%zu\t\"%s(rabs)\"\t%s", i + 2 * mds.count, model_name, buffer);
	}
	pclose(pipe);
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $15}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			fprintf(r_temp_file, "%zu\t\"%s(rrel)\"\t%s", i + 3 * mds.count, model_name, buffer);
	}
	pclose(pipe);
	fclose(w_temp_file);
	fclose(r_temp_file);
	fprintf(gp, "set boxwidth 0.5\n");
	fprintf(gp, "set style fill solid\n");
	fprintf(gp, "plot 'wtemp.txt' using 1:3:xtic(2) with boxes title 'write(%zu)', 'rtemp.txt' using 1:3:xtic(2) with boxes title 'read(%zu)'\n", nlaunch, nlaunch);
	pclose(gp);
	
	if ((gp = popen("gnuplot -p", "w")) == NULL) {
		fprintf(stderr, "Cannot open gnuplot\n");
		return EXIT_FAILURE;
	}
	if ((w_temp_file = fopen("wtemp.txt", "w")) == NULL) {
		fprintf(stderr, "Cannot open temp file\n");
		return EXIT_FAILURE;
	}		
	if ((r_temp_file = fopen("rtemp.txt", "w")) == NULL) {
		fprintf(stderr, "Cannot open temp file\n");
		return EXIT_FAILURE;
	}
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $8}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	size_t j = 0;
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			if (strcmp(model_name, "RAM") != 0)
				fprintf(w_temp_file, "%zu\t\"%s\"\t%s", j++, model_name, buffer);
	}
	pclose(pipe);
	sprintf(buffer, "cat %s | gawk -F \',\' \'{print $13}\' | uniq", CSVFILENAME);
	if ((pipe = popen(buffer, "r")) == NULL) {
		return EXIT_FAILURE;
	}
	for (size_t i = 0; i < mds.count; ++i) {
		const char* model_name = (mds.mdis[i].type != RAM) ? SMDI_CAST(mds.mdis + i)->model_name : RMDI_CAST(mds.mdis + i)->model_name;
		if (fgets(buffer, sizeof(buffer), pipe) != NULL)
			if (strcmp(model_name, "RAM") != 0)
				fprintf(r_temp_file, "%zu\t\"%s\"\t%s", j++, model_name, buffer);
	}
	pclose(pipe);
	fclose(w_temp_file);
	fclose(r_temp_file);
	fprintf(gp, "set boxwidth 0.5\n");
	fprintf(gp, "set style fill solid\n");
	fprintf(gp, "plot 'wtemp.txt' using 1:3:xtic(2) with boxes title 'write(%zu)', 'rtemp.txt' using 1:3:xtic(2) with boxes title 'read(%zu)'\n", buffer_size, buffer_size);
	pclose(gp);
	if (remove("wtemp.txt") != 0 || remove("rtemp.txt") != 0) {
		fprintf(stderr, "Cannot remove temp file\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
	
void write_csv(FILE* csv, size_t block_size, size_t buffer_size, struct memory_device_info* mdi)
{
	const char* model_name = (mdi->type != RAM) ? SMDI_CAST(mdi)->model_name : RMDI_CAST(mdi)->model_name;
	struct statistics* stats = (mdi->type != RAM) ? &SMDI_CAST(mdi)->stats : &RMDI_CAST(mdi)->stats;
	for (size_t i = 0; i < stats->nlaunch; ++i) {
		fprintf(csv, FORMAT, model_name, 
												 block_size,
												 buffer_size,
												 i + 1,
												 stats->w_launch_time[i],
												 stats->w_average_time,
												 stats->w_bandwidth,
												 stats->w_absolute_error,
												 stats->w_relative_error,
												 stats->r_launch_time[i],	
												 stats->r_average_time,
												 stats->r_bandwidth,
												 stats->r_absolute_error,
												 stats->r_relative_error);
	}					
}
