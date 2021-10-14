#include "../../include/test_memory/test_memory.h"
#include "../../include/timer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define CONSTANT 1000000
#define PATH_MAX 255

void __test_ram_memory_device(void* block, size_t block_size, size_t buffer_size, double max_clock_frequency, struct ram_memory_device_info* rmdi)
{
	void* dest;
	if ((dest = malloc(block_size)) == NULL)
		return;
	double* w_launch_time = rmdi->stats.w_launch_time, *r_launch_time = rmdi->stats.r_launch_time;
	double start, finish, w_average_time = 0, w_absolute_error = 0, r_average_time = 0, r_absolute_error = 0;
	size_t nlaunch = rmdi->stats.nlaunch;
	char* _block = block, *_dest = dest;
	for (size_t i = 0; i < nlaunch; ++i) {
		r_launch_time[i] = w_launch_time[i] = 0;
		for (size_t j = 0; j < block_size; j += buffer_size) {
			start = rdtsc();
			memcpy(_block, _dest, buffer_size);
			finish = rdtsc();
			w_launch_time[i] += (finish - start) / max_clock_frequency;
			w_average_time += (finish - start) / max_clock_frequency;	
			start = rdtsc();
			memcpy(_dest, _block, buffer_size);
			finish = rdtsc();
			r_launch_time[i] += (finish - start) / max_clock_frequency;
			r_average_time += (finish - start) / max_clock_frequency;
			_block += buffer_size; _dest += buffer_size;
		}
		_block = block; _dest = dest;
	}
	free(dest);
	w_average_time /= nlaunch;
	r_average_time /= nlaunch;
	for (size_t i = 0; i < nlaunch; ++i) {
		w_absolute_error += fabs(w_launch_time[i] - w_average_time);
		r_absolute_error += fabs(r_launch_time[i] - r_average_time);
	}	
	rmdi->stats.w_average_time = w_average_time;
	rmdi->stats.w_bandwidth = block_size / w_average_time * CONSTANT;
	rmdi->stats.w_absolute_error = w_absolute_error;
	rmdi->stats.w_relative_error = w_absolute_error / w_average_time * 100;
	rmdi->stats.r_average_time = r_average_time;
	rmdi->stats.r_bandwidth = block_size / r_average_time * CONSTANT;
	rmdi->stats.r_absolute_error = r_absolute_error;
	rmdi->stats.r_relative_error = r_absolute_error / r_average_time * 100;
}

void __test_storage_memory_device(void* block, size_t block_size, size_t buffer_size, double max_clock_frequency, struct storage_memory_device_info* smdi)
{
	FILE* temp_file;
	char path[PATH_MAX]; 
	sprintf(path, "%s/temp_file.bin", smdi->mountpoint); 
	if ((temp_file = fopen(path, "wb")) == NULL)
		return;
	double* w_launch_time = smdi->stats.w_launch_time, *r_launch_time = smdi->stats.r_launch_time;
	double start, finish, w_average_time = 0, w_absolute_error = 0, r_average_time = 0, r_absolute_error = 0;
	size_t nlaunch = smdi->stats.nlaunch;
	char* _block = block;
	for (size_t i = 0; i < nlaunch; ++i) {
		r_launch_time[i] = w_launch_time[i] = 0;
		for (size_t j = 0; j < block_size; j += buffer_size) {
			start = rdtsc();
			fwrite(_block, buffer_size, 1, temp_file);
			finish = rdtsc();
			w_launch_time[i] += (finish - start) / max_clock_frequency;
			w_average_time += (finish - start) / max_clock_frequency;
			fseek(temp_file, -((long)buffer_size), SEEK_CUR);
			start = rdtsc();
			fread(_block, buffer_size, 1, temp_file);
			finish = rdtsc();
			r_launch_time[i] += (finish - start) / max_clock_frequency;
			r_average_time += (finish - start) / max_clock_frequency;
			_block += buffer_size;
		}
		_block = block;
	}
	fclose(temp_file); remove(path);
	w_average_time /= nlaunch;
	r_average_time /= nlaunch;
	for (size_t i = 0; i < nlaunch; ++i) {
		w_absolute_error += fabs(w_launch_time[i] - w_average_time);
		r_absolute_error += fabs(r_launch_time[i] - r_average_time);
	}
	smdi->stats.w_average_time = w_average_time;
	smdi->stats.w_bandwidth = block_size / w_average_time * CONSTANT;
	smdi->stats.w_absolute_error = w_absolute_error;
	smdi->stats.w_relative_error = w_absolute_error / w_average_time * 100;
	smdi->stats.r_average_time = r_average_time;
	smdi->stats.r_bandwidth = block_size / r_average_time * CONSTANT;
	smdi->stats.r_absolute_error = r_absolute_error;
	smdi->stats.r_relative_error = r_absolute_error / r_average_time * 100;
}

void test_memory(void* block, size_t block_size, size_t buffer_size, double max_clock_frequency, struct memory_device_info* mdi)
{
	if (mdi->type != RAM)
		__test_storage_memory_device(block, block_size, buffer_size, max_clock_frequency, SMDI_CAST(mdi));
	else
		__test_ram_memory_device(block, block_size, buffer_size, max_clock_frequency, RMDI_CAST(mdi));
}
