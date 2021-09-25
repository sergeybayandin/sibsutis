#include "../../include/test_cpu/test_cpu.h"
#include "../../include/timer.h"
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

#define DEFINE_TEST_CPU(suffix, op_type) \
	void test_cpu_##suffix(struct statistics* stats, double max_clock_frequency, \
		op_type (*task)(op_type)) \
	{ \
		const size_t nlaunch = stats->nlaunch; \
		double* launch_time = stats->launch_time; \
		double start, finish, average_time = 0, absolute_error = 0, task_perfomance = 0, p = 1.0 / nlaunch; \
		op_type operand; \
		for (size_t i = 0; i < nlaunch; ++i) { \
			operand = rand() % UPPER - DOWN; \
			start = rdtsc(); \
			task(operand); \
			finish = rdtsc(); \
			average_time += (launch_time[i] = (finish - start) / max_clock_frequency); \
		} \
		average_time /= nlaunch; \
		stats->average_time = average_time; \
		for (size_t i = 0; i < nlaunch; ++i) { \
			absolute_error += fabs(launch_time[i] - average_time); \
			task_perfomance += 1.0 / (launch_time[i] * p); \
		} \
		stats->absolute_error = absolute_error; \
		stats->relative_error = absolute_error / average_time * 100; \
		stats->task_perfomance = task_perfomance; \
	} \

DEFINE_TEST_CPU(f, float)
DEFINE_TEST_CPU(d, double)
DEFINE_TEST_CPU(ld, long double)

void test_cpu(struct statistics* stats, double max_clock_frequency, int typeid, int taskid)
{
	switch (typeid) {
		case FLOAT : {
			switch (taskid) {
				case SIN : {
					test_cpu_f(stats, max_clock_frequency, sinf);
					break;
				}
				case COS : {
					test_cpu_f(stats, max_clock_frequency, cosf);
					break;
				}
				case TAN : {
					test_cpu_f(stats, max_clock_frequency, tanf);
				}
			} 
			break;
		}
		case DOUBLE : {			
			switch (taskid) {
				case SIN : {
					test_cpu_d(stats, max_clock_frequency, sin);
					break;
				}
				case COS : {
					test_cpu_d(stats, max_clock_frequency, cos);
					break;
				}
				case TAN : {
					test_cpu_d(stats, max_clock_frequency, tan);
				}
			} 
			break;
		}
		case LONG_DOUBLE : {
			switch (taskid) {
				case SIN : {
					test_cpu_ld(stats, max_clock_frequency, sinl);
					break;
				}
				case COS : {
					test_cpu_ld(stats, max_clock_frequency, cosl);
					break;
				}
				case TAN : {
					test_cpu_ld(stats, max_clock_frequency, tanl);
				}
			} 
		}
	}
}
