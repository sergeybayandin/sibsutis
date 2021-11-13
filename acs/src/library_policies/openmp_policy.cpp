#include "../../include/library_policies/openmp_policy.h"
#include <omp.h>

std::chrono::duration<double>
openmp_policy::compute(const std::vector<double> &a, const std::vector<double> &b,
	std::vector<double> &c, std::size_t n, std::size_t threads_count)
{
	auto a_last {std::end(a)};
	auto b_first {std::begin(b)}, b_last {std::next(b_first, n)};
	decltype(a_last) a_first, row_first, row_last, col_first, col_it;
	auto c_it {std::begin(c)};	
	omp_set_num_threads(threads_count);
	auto start {std::chrono::steady_clock::now()};
#pragma omp parallel for shared(c_it) private(a_first, col_first, row_first, row_last)
	for (a_first = std::begin(a); a_first < a_last; a_first += n) {
		row_last = std::next(a_first, n);
		for (col_first = b_first; col_first < b_last; ++col_first) {
			col_it = col_first;
			*c_it = {};
			for (row_first = a_first; row_first < row_last; ++row_first) {
				*c_it += *row_first * *col_it;
				std::advance(col_it, n);
			}
			++c_it;
		}
	}
	auto finish {std::chrono::steady_clock::now()};
	return finish - start;
}
