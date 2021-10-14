#ifndef __MEMORY_STATISTICS_H__
#define __MEMORY_STATISTICS_H__

#include <stddef.h>

struct statistics
{
  size_t  nlaunch;
  double* w_launch_time;
  double  w_average_time;
  double  w_bandwidth;
  double  w_absolute_error;
  double  w_relative_error;
  double* r_launch_time;
  double  r_average_time;
  double  r_bandwidth;
  double  r_absolute_error;
  double  r_relative_error;
};

#endif // __MEMORY_STATISTICS_H__
