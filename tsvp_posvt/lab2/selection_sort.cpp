#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

template <class BidirectionalIterator, class Compare>
std::pair<std::size_t, std::size_t>
selection_sort(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
{
	std::size_t m {}, n {};
	for (auto _last {std::prev(last)}; first != _last; ++first) {
		auto it {first};
		for (auto _first {std::next(first)}; _first != last; ++_first)
			if ((++m, comp(*_first, *it)))
				it = _first;
		if (it != first)
			(n += 3, std::iter_swap(it, first));
	}
	return {m, n};
}

int main()
{
	std::string s; std::getline(std::cin, s);
	std::istringstream is {s};
	std::istream_iterator<int> begin {is}, end;
	std::vector<int> v(begin, end);
	const auto [m, n] {selection_sort(std::begin(v), std::end(v), std::less<int>{})};
	for (auto val : v)
		std::cout << val << '\t';
	std::cout << '\n';
	std::cout << "m = " << m << '\n';
	std::cout << "n = " << n << '\n';
	std::cout << "m + n = " << m + n << '\n';
	return EXIT_SUCCESS;
}
