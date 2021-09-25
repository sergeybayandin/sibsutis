#include "../../include/test_cpu/cpuinfo.h"
#include <stdio.h>

int get_processor_model_name(char* buffer, size_t size)
{
	FILE* command;
	if ((command = popen("cat /proc/cpuinfo | grep -oP \'model name	:\\s*\\K.+\'", "r")) == NULL)
		return -1;
	int ch;
	size_t i;
	for (i = 0; i < size && (ch = getc(command)) != '\n' && ch != EOF; ++i)
		buffer[i] = ch;
	buffer[i] = '\0';
	pclose(command);
	return 0;
}

int get_max_clock_frequency(double* max_clock_frequency)
{
	FILE* command;
	if ((command = popen("lscpu | grep -oP \'CPU max MHz:\\s*\\K.+\'", "r")) == NULL) 
		return -1;
	fscanf(command, "%lf", max_clock_frequency);
	*max_clock_frequency *= 1000000;
	pclose(command);
	return 0;
}
