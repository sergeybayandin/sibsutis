#ifndef __SLOW_MULTIPLICATION_H__
#define __SLOW_MULTIPLICATION_H__

#include <vector>
#include <utility>

namespace rgz
{
	std::pair<std::size_t, std::size_t>
	slow_multiplication(
		const std::vector<int>&,
		const std::vector<int>&,
		std::vector<int>&,
		std::size_t
	);
}

#endif // __SLOW_MULTIPLICATION_H__
