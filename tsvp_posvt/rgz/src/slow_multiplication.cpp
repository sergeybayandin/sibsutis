#include "../include/slow_multiplication.h"

namespace rgz
{
	std::pair<std::size_t, std::size_t>
	slow_multiplication(
		const std::vector<int> &a,
		const std::vector<int> &b,
		std::vector<int> &c,
		std::size_t n
	)
	{
		std::size_t multiplications {}, additions {};
		auto 
			a_first {a.begin()}, 
			a_last {a.end()},
			b_first {b.begin()},
			b_last {b_first + n};
		auto c_it {c.begin()};
		decltype(b_first) col_it;
		decltype(a_first) row_first, row_last;
		for (; a_first < a_last; a_first = row_last) {
			row_last = a_first + n;
			for (; b_first < b_last; ++b_first) {
				*c_it = {};
				col_it = b_first;
				row_first = a_first;
				for (; row_first < row_last; ++row_first, col_it += n) {
					*c_it += *row_first * *col_it;
					++multiplications;
					++additions;
				}
				++c_it;
			}
			b_first = b.begin();
		}
		return {multiplications, additions};
	}
}
