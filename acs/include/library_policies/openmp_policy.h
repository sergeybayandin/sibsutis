#ifndef __OPENMP_POLICY_H__
#define __OPENMP_POLICY_H__

#include <chrono>
#include <vector>

struct openmp_policy
{
	static std::chrono::duration<double>
	compute(const std::vector<double> &, const std::vector<double> &,
		std::vector<double> &, std::size_t, std::size_t);
};

#endif // __OPENMP_POLICY_H__
