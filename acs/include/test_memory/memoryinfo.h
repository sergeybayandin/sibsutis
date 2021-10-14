#ifndef __MEMORYINFO_H__
#define __MEMORYINFO_H__

#include "memory_statistics.h"
#include <stddef.h>

#define MAX_MODEL_NAME_LENGTH 64
#define MAX_MOUNTPOINT_LENGTH 64

#define HDD		1
#define SSD 	2
#define RAM 	3
#define FLASH 4

struct ram_memory_device_info
{
	char model_name[MAX_MODEL_NAME_LENGTH];
	struct statistics stats;
};

struct storage_memory_device_info
{
	char model_name[MAX_MODEL_NAME_LENGTH];
	char mountpoint[MAX_MOUNTPOINT_LENGTH];
	struct statistics stats;
};

struct memory_device_info
{
	int type;
	void* mdi;
};

struct memory_devices_info
{
	struct memory_device_info* mdis;
	size_t count;
};

struct memory_devices_info get_memory_devices_info(int, int, int, int);

#define RMDI_CAST(mdis) \
	(((mdis)->type == RAM) ? (struct ram_memory_device_info*)((mdis)->mdi) : NULL)

#define SMDI_CAST(mdis) \
	(((mdis)->type != RAM) ? (struct storage_memory_device_info*)((mdis)->mdi) : NULL)

#endif // __MEMORYINFO_H__
