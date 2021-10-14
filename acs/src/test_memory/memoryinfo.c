#include "../../include/test_memory/memoryinfo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_BUFFER_SIZE 64

int __reserve(struct memory_device_info** mdis, size_t capacity)
{
	return ((*mdis = realloc(*mdis, capacity * sizeof **mdis)) == NULL ? -1 : 0);
}

int __getline(FILE* fin, char* s, size_t size)
{
	int ch;
	size_t i;
	for (i = 0; (ch = getc(fin)) != EOF && ch != '\n'; ++i) {
		if (i >= size - 1)
			return -1;
		s[i] = ch;
	}
	s[i] = '\0';
	return 0;
}

struct memory_devices_info get_memory_devices_info(int ram, int hdd, int ssd, int flash)
{
	int ch, number, max_number;
	FILE* pipe, *fin;
	char buffer[MAX_BUFFER_SIZE], buffer1[MAX_BUFFER_SIZE];
	size_t capacity = 0, count = 0, i, j, max_size = 0, size;
	struct memory_device_info* mdis = NULL;
	struct memory_devices_info mds = {mdis, count};
	struct storage_memory_device_info* smdi;
	if (ram == RAM) {
		if (__reserve(&mdis, capacity = 1) != 0) {
			mds.mdis = mdis; mds.count = count;
			return mds;
		}
		mdis[count].type = RAM;
		if ((mdis[count].mdi = malloc(sizeof(struct ram_memory_device_info))) == NULL) {
			mds.mdis = mdis; mds.count = count;
			return mds;
		}
		strcpy(((struct ram_memory_device_info*)mdis[count].mdi)->model_name, "RAM");
		++count;
	}
	if (hdd == HDD) {
		if ((pipe = popen("lsblk -d -o NAME,TRAN,ROTA | grep 1 | grep sata | gawk \' { print $1 } \'", "r")) == NULL) {
			mds.mdis = mdis; mds.count = count;
			return mds;
		}
		for (i = 0;;) {
			if (i >= MAX_BUFFER_SIZE) {
				mds.mdis = mdis; mds.count = count;
				return mds;
			}
			if ((ch = getc(pipe)) == EOF && i == 0)
				break;
			if (ch == '\n' || ch == EOF) {
				buffer[i] = '\0';
				for (number = 1, max_number = 1; ; ++number) {
					sprintf(buffer1, "/sys/block/%s/%s%d/size", buffer, buffer, number);
					if ((fin = fopen(buffer1, "r")) == NULL)
						break;
					fscanf(fin, "%zu", &size);
					if (max_size < size) {
						max_size = size;
						max_number = number;
					}
					fclose(fin);
				}
				if (count >= capacity && __reserve(&mdis, (capacity == 0 ? (capacity = 1) : (capacity *= 2))) != 0) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					return mds;
				}				
				if ((mdis[count].mdi = malloc(sizeof(struct storage_memory_device_info))) == NULL) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					fclose(fin);
					return mds;
				}
				smdi = (struct storage_memory_device_info*)mdis[count].mdi;
				sprintf(buffer1, "/sys/block/%s/device/model", buffer);
				if ((fin = fopen(buffer1, "r")) == NULL)
					break;
				if (__getline(fin, smdi->model_name, MAX_MODEL_NAME_LENGTH) != 0) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					fclose(fin);
					return mds;
				}
				fclose(fin);
				sprintf(buffer1, "lsblk -o NAME,MOUNTPOINT | grep \'%s%d\' | gawk \' { print $2 } \'", buffer, max_number);
				if ((fin = popen(buffer1, "r")) == NULL) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					return mds;
				}
				if (__getline(fin, smdi->mountpoint, MAX_MOUNTPOINT_LENGTH) != 0) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					pclose(fin);
					return mds;
				}
				pclose(fin);
				mdis[count].type = HDD;
				++count;
				i = 0;
			} else {
				buffer[i++] = ch;
			}
		}
		pclose(pipe);
	}
	if (ssd == SSD) {
		if ((pipe = popen("lsblk -d -o NAME,TRAN,ROTA | grep 0 | grep sata | gawk \' { print $1 } \'", "r")) == NULL) {
			mds.mdis = mdis; mds.count = count;
			return mds;
		}
		for (i = 0;;) {
			if (i >= MAX_BUFFER_SIZE) {
				mds.mdis = mdis; mds.count = count;
				return mds;
			}
			if ((ch = getc(pipe)) == EOF && i == 0)
				break;
			if (ch == '\n' || ch == EOF) {
				buffer[i] = '\0';
				for (number = 1, max_number = 1; ; ++number) {
					sprintf(buffer1, "/sys/block/%s/%s%d/size", buffer, buffer, number);
					if ((fin = fopen(buffer1, "r")) == NULL)
						break;
					fscanf(fin, "%zu", &size);
					if (max_size < size) {
						max_size = size;
						max_number = number;
					}
					fclose(fin);
				}	
				if (count >= capacity && __reserve(&mdis, (capacity == 0 ? (capacity = 1) : (capacity *= 2))) != 0) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					return mds;
				}				
				if ((mdis[count].mdi = malloc(sizeof(struct storage_memory_device_info))) == NULL) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					fclose(fin);
					return mds;
				}
				smdi = (struct storage_memory_device_info*)mdis[count].mdi;	
				sprintf(buffer1, "/sys/block/%s/device/model", buffer);
				if ((fin = fopen(buffer1, "r")) == NULL)
					break;
				if (__getline(fin, smdi->model_name, MAX_MODEL_NAME_LENGTH) != 0) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					fclose(fin);
					return mds;
				}
				fclose(fin);
				sprintf(buffer1, "lsblk -o NAME,MOUNTPOINT | grep \'%s%d\' | gawk \' { print $2 } \'", buffer, max_number);
				if ((fin = popen(buffer1, "r")) == NULL) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					return mds;
				}
				mdis[count].type = SSD;
				if (__getline(fin, smdi->mountpoint, MAX_MOUNTPOINT_LENGTH) != 0) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					pclose(fin);
					return mds;
				}
				pclose(fin);
				++count;
				i = 0;
			} else {
				buffer[i++] = ch;
			}
		}
		pclose(pipe);
	}
	if (flash == FLASH)	{
		if ((pipe = popen("lsblk -d -o NAME,TRAN,ROTA | grep 1 | grep usb | gawk \' { print $1 } \'", "r")) == NULL) {
			mds.mdis = mdis; mds.count = count;
			return mds;
		}
		for (i = 0;;) {
			if (i >= MAX_BUFFER_SIZE) {
				mds.mdis = mdis; mds.count = count;
				return mds;
			}
			if ((ch = getc(pipe)) == EOF && i == 0)
				break;
			if (ch == '\n' || ch == EOF) {
				buffer[i] = '\0';
				for (number = 1, max_number = 1; ; ++number) {
					sprintf(buffer1, "/sys/block/%s/%s%d/size", buffer, buffer, number);
					if ((fin = fopen(buffer1, "r")) == NULL)
						break;
					fscanf(fin, "%zu", &size);
					if (max_size < size) {
						max_size = size;
						max_number = number;
					}
					fclose(fin);
				}	
				if (count >= capacity && __reserve(&mdis, (capacity == 0 ? (capacity = 1) : (capacity *= 2))) != 0) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					return mds;
				}				
				if ((mdis[count].mdi = malloc(sizeof(struct storage_memory_device_info))) == NULL) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					fclose(fin);
					return mds;
				}
				smdi = (struct storage_memory_device_info*)mdis[count].mdi;	
				sprintf(buffer1, "/sys/block/%s/device/model", buffer);
				if ((fin = fopen(buffer1, "r")) == NULL)
					break;
				if (__getline(fin, smdi->model_name, MAX_MODEL_NAME_LENGTH) != 0) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					fclose(fin);
					return mds;
				}
				fclose(fin);
				sprintf(buffer1, "lsblk -o NAME,MOUNTPOINT | grep %s%d | gawk \' { print $2 } \'", buffer, max_number);
				if ((fin = popen(buffer1, "r")) == NULL) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					return mds;
				}
				mdis[count].type = FLASH;
				memset(smdi->mountpoint, 0x00, MAX_MOUNTPOINT_LENGTH * sizeof *smdi->mountpoint);
				if (__getline(fin, smdi->mountpoint, MAX_MOUNTPOINT_LENGTH) != 0) {
					mds.mdis = mdis; 
					mds.count = count;
					pclose(pipe);
					pclose(fin);
					return mds;
				}
				if (*smdi->mountpoint == '\0') {
					sprintf(smdi->mountpoint, "/mnt/%s", smdi->model_name);
					for (j = 0; smdi->mountpoint[j] != '\0'; ++j)
						if (smdi->mountpoint[j] == ' ')
							smdi->mountpoint[j] = '_';
					sprintf(buffer1, "sudo mkdir -p %s", smdi->mountpoint);
					system(buffer1);
					sprintf(buffer1, "sudo mount /dev/%s%d %s", buffer, max_number, smdi->mountpoint);
					system(buffer1);
				}
				pclose(fin);
				++count;
				i = 0;
			} else {
				buffer[i++] = ch;
			}
		}
		pclose(pipe);
	}
	mds.mdis = mdis; mds.count = count;
	return mds;
}
