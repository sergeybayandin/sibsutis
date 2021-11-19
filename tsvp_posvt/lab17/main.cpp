#include "../include/brackets.h"
#include <iostream>

int main()
{
	std::size_t n; std::cin >> n; ++n;
	std::vector<std::size_t> dims(n);
	for (std::size_t i {}; i < n; ++i)
		std::cin >> dims[i];
	const auto obs {brackets_problem(dims)};
	std::cout << obs.complexity() << '\n' << obs << '\n';
	return 0;
}
