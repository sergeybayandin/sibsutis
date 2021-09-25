#include "../include/timer.h"

uint64_t rdtsc()
{
	uint32_t edx, eax;
	asm volatile("rdtsc\n" : "=a" (eax), "=d" (edx));
	return ((uint64_t)edx << 32) | eax;
}
