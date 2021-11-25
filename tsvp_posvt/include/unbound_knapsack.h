#ifndef __UNBOUND_KNAPSACK_H__
#define __UNBOUND_KNAPSACK_H__

#include <vector>
#include <utility>

namespace dp
{
	namespace details
	{
		using weight_type = std::size_t;
		using cost_type = double;
		using things_storage = std::vector<std::pair<weight_type, cost_type>>;
		using dp_storage = std::vector<std::pair<cost_type, things_storage::const_iterator>>;
		using loot_storage = std::vector<std::size_t>;
		using result_type = std::pair<cost_type, loot_storage>;
	}
	details::result_type
	unbound_knapsack_problem(details::weight_type, const details::things_storage&);
}

#endif // __UNBOUND_KNAPSACK_H__
