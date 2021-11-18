#include "../include/backpack.h"

std::vector<std::pair<std::size_t, double>>
backpack(std::size_t m, const std::vector<std::pair<std::size_t, double>> &things)
{
	const auto cols {m + 1};
	std::vector<double>	bps(cols * (things.size() + 1), {});
	
	auto bps_last  {std::end(bps)};
	auto things_it {std::begin(things)};
	auto bps_first {std:next(std::begin(bps), cols)}; 
	decltype(bps_first)	col_last, col_first, lhs_it, rhs_it, prev_bps_first;

	for (; bps_first < bps_last; bps_first = col_last, ++things_it) {
		col_last = std::next(bps_first, cols);
		prev_bps_first = std::prev(bps_first, cols);
		for (col_first = std::next(bps_first); col_first < col_last; ++col_first) {
			lhs_it = std::prev(col_first, cols);
			rhs_it = std::prev(lhs_it, things_it->first);
			*col_first = (rhs_it >= prev_bps_first) ? 
				std::max(*lhs_it, *rhs_it + things_it->second) : *lhs_it;
		}
	}

	std::vector<std::pair<std::size_t, double>> loot;
	loot.reserve(things.size());
	
	bps_first = std::prev(bps_last);

	for (--things_it; *bps_first; --things_it) {
		bps_last = std::prev(bps_first, cols);
		if (*bps_last != *bps_first) {
			loot.push_back(*things_it);
			bps_first = std::prev(bps_last, things_it->first);
		} else {
			bps_first = bps_last;
		}
	}

	return loot;
}
