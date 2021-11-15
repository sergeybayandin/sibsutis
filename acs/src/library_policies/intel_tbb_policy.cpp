#include "../../include/library_policies/intel_tbb_policy.h"
#include "tbb/global_control.h"
#include "tbb/parallel_for.h"

std::chrono::duration<double>
intel_tbb_policy::compute(const std::vector<double> &a, const std::vector<double> &b,
	std::vector<double> &c, std::size_t n, std::size_t threads_count)
{
	tbb::global_control global_limit(tbb::global_control::max_allowed_parallelism, threads_count);
	auto start {std::chrono::steady_clock::now()};
	tbb::parallel_for(std::size_t {}, n, [&](auto i) {
		tbb::parallel_for(std::size_t {}, n, [&](auto j) {
			tbb::parallel_for(std::size_t {}, n, [&] (auto k) {
				c[i * n + j] += a[i * n + k] * b[k * n + j];
			});
		});
	});
	auto finish {std::chrono::steady_clock::now()};
	return finish - start;
}
