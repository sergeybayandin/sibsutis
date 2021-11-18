#include "../include/backpack.h"
#include <iostream>
#include <numeric>

int main()
{
	std::size_t m, nthings;
	std::cin >> m >> nthings;
	std::vector<std::pair<std::size_t, double>> things(nthings);
	for (auto &[mi, ci] : things)
		std::cin >> mi >> ci;
	auto loot {backpack(m, things)};
	for (const auto &[mi, ci] : loot)
		std::cout << mi << '\t' << ci << '\n';
	auto [weight, cost] {
		std::accumulate(std::begin(loot), std::end(loot), std::make_pair<std::size_t, double>(0, 0.0), 
		[](const auto &lhs, const auto &rhs) {
			return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
		})
	};
	std::cout << "weight: " << weight << '\n';
	std::cout << "cost: " << cost << '\n';
	return 0;
}
