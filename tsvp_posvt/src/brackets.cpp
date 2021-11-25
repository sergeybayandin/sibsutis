#include "../include/brackets.h"
#include <numeric>
#include <algorithm>

__brackets_observer
brackets_problem(const std::vector<std::size_t> &dims)
{
	auto n {std::size(dims) - 1};
	std::vector<std::size_t> dp(
		n * n,
		std::numeric_limits<std::size_t>::max()
	);

	auto dp_first {std::begin(dp)}, dp_last {std::end(dp)};
	const auto i {n + 1};

	for (; dp_first < dp_last; dp_first += i)
		*dp_first = {};
	
	std::size_t lrhs_val, min_val;
	decltype(dp_first) row_it, col_it, lopt_it, ropt_it;
	typename std::vector<std::size_t>::const_iterator lhs_it, rhs_it, mid_it;
	std::vector<std::pair<std::size_t, std::size_t>> optimals;
	
	optimals.reserve(3 * n);

	for (decltype(n) sz {1}; sz < n; ++sz, dp_last -= n) {
		lhs_it = std::begin(dims);
		rhs_it = lhs_it + sz + 1;
		for (dp_first = std::begin(dp) + sz; dp_first < dp_last; dp_first += i) {
			col_it = dp_first + n;
			lrhs_val = *lhs_it * *rhs_it;
			mid_it = std::begin(dims) + (dp_first - std::begin(dp)) / n + 1;
			for (row_it = dp_first - sz; row_it < dp_first; ++row_it, col_it += n, ++mid_it) {
				min_val = *row_it + *col_it + lrhs_val * *mid_it;
				if (min_val < *dp_first) {
					*dp_first = min_val;
					lopt_it = row_it;
					ropt_it = col_it;
				}
			}
			optimals.emplace_back(
				(lopt_it - std::begin(dp)) / n,
				lopt_it - (dp_first - sz - (dp_first - std::begin(dp)) / n)
			);
			optimals.emplace_back(
				(ropt_it - std::begin(dp)) / n,
				dp_first - (dp_first - sz - (dp_first - std::begin(dp)) / n)
				
			);
			++lhs_it;
			++rhs_it;
		}
	}
	
	return { dp[n - 1], std::move(optimals) };
}

std::ostream &operator<<(std::ostream &strm, const __brackets_observer &obs)
{
	std::string out;
	obs.__show_brackets_recursive(
		out, 
		std::cend(obs.storage_) - 1, 
		std::cbegin(obs.storage_)
	);
	std::reverse(std::begin(out), std::end(out));
	strm << out;
	return strm;
}

void __brackets_observer::__show_brackets_recursive(std::string &out, 
	auto last, auto first) const noexcept
{
	if (first > last)
		return;
	
	const auto [la, lb] {*last};
	
	out += ")";

	if (la != lb) {
		__show_brackets_recursive(
			out, 
			__find_optimal(last, first, la, lb), 
			first
		);
	} else {
		out += std::to_string(la + 1) + std::string{'M'};
	}
	
	if (first >= last)
		return;

	const auto [ra, rb] {*(last - 1)};

	if (ra != rb) {
		__show_brackets_recursive(
			out, 
			__find_optimal(last, first, ra, rb), 
			first
		);
	} else {
		out += std::to_string(ra + 1) + std::string{'M'};
	}
	
	out += "(";
}

auto
__brackets_observer::__find_optimal(auto last, auto 
	first, std::size_t a, std::size_t b) const noexcept
{
		auto next_it {last - 2}, prev_it {next_it - 1};
		while (first <= prev_it && 
			(prev_it->first != a || next_it->second != b)) {
			--next_it;
			--prev_it;
		}
		return next_it;
}
