#include "../include/unbound_knapsack.h"

namespace dp
{
	using namespace details;

	result_type
	unbound_knapsack_problem(weight_type m, const things_storage &things)
	{
		cost_type curr_cost;
		dp_storage dp(m + 1, {0.0, things.end()});
		auto dp_first {dp.begin() + 1}, dp_last {dp.end()};
		auto things_first {things.begin()}, things_last {things.end()};
		for (; dp_first < dp_last; ++dp_first) {
			dp_first->first = (dp_first - 1)->first;
			for (; things_first < things_last; ++things_first) {
				if (dp_first - things_first->first < dp.begin()) {
					continue;
				}
				curr_cost = (dp_first - things_first->first)->first + things_first->second;
				if (curr_cost > dp_first->first) {
					dp_first->first = curr_cost;
					dp_first->second = things_first;
				}
			} 
			things_first = things.begin();
		}
		loot_storage loot(things.size());
		auto dp_it {dp_last - 1};
		for (dp_first = dp.begin(); dp_it >= dp_first; ) {
			if (dp_it->second != things_last) {
				++loot[dp_it->second - things.begin()];
				dp_it -= dp_it->second->first;
			} else {
				--dp_it;
			}
		}
		return {(dp_last - 1)->first, std::move(loot)};
	}
}
