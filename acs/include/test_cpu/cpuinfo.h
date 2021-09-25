#ifndef __CPUINFO_H__
#define __CPUINFO_H__

#include <stddef.h>

int get_processor_model_name(char*, size_t);
int get_max_clock_frequency(double*);

#endif // __CPUINFO_H__
