#ifndef __TESTCPU_H__
#define __TESTCPU_H__

#include <stddef.h>

#define FLOAT 	 		0
#define DOUBLE 		  1
#define LONG_DOUBLE 2

#define SIN 0
#define COS 1
#define TAN 2

#define DOWN 				 1000
#define UPPER				 2000;

struct statistics
{	
	size_t  nlaunch;
	double* launch_time;
	double  average_time;
	double 	absolute_error;
	double  relative_error;
	double 	task_perfomance;
};

void test_cpu(struct statistics*, double, int, int);

#endif // __TESTCPU_H__
