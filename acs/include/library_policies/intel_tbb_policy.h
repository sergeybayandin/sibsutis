#ifndef __INTEL_TBB_POLICY_H__
#define __INTEL_TBB_POLICY_H__

#include <vector>
#include <chrono>

struct intel_tbb_policy
{
	static std::chrono::duration<double>
	compute(const std::vector<double> &, const std::vector<double> &,
		std::vector<double> &, std::size_t, std::size_t);
};

#endif // __INTEL_TBB_POLICY_H__
