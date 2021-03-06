#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>

template <class BidirectionalIterator, class Compare>
std::pair<std::size_t, std::size_t>
bubble_sort(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
{
	std::size_t m {}, n {};
	if (first == last)
		return {m, n};
	while (true) {
		auto sorted {true};
		--last;
		for (auto _first {first}; _first != last; ++_first) {
			if ((++m, comp(*std::next(_first), *_first))) {
				std::iter_swap(std::next(_first), _first);
				n += 3;
				sorted = false;
			}
		}
		if (sorted)
			break;
	}
	return {m, n};
}

int main()
{
	std::string s; std::getline(std::cin, s);
	std::istringstream is {s};
	std::istream_iterator<int> begin {is}, end;
	std::vector<int> v(begin, end);
	const auto [m, n] {bubble_sort(std::begin(v), std::end(v), std::less<int>{})};
	for (auto val : v)
		std::cout << val << '\t';
	std::cout << '\n';
	std::cout << "m = " << m << '\n';
	std::cout << "n = " << n << '\n';
	std::cout << "m + n = " << m + n << '\n';
	return EXIT_SUCCESS;
}
