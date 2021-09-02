#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <type_traits>

template <class ForwardIterator, class Compare>
void merge(ForwardIterator first, ForwardIterator mid,
	ForwardIterator last, Compare comp, std::size_t& m, std::size_t& n)
{
	std::vector<std::remove_reference_t<decltype(*first)>> merged;
	merged.reserve(std::distance(first, last));
	auto l_first {first}, r_first {mid};
	while (l_first != mid && r_first != last) {
		auto& it {(++m, comp(*l_first, *r_first)) ? l_first : r_first};
		merged.push_back(std::move(*it)); ++n;
		++it;
	}
	std::move(l_first, mid, std::back_inserter(merged));
	std::move(r_first, last, std::back_inserter(merged));
	std::move(std::begin(merged), std::end(merged), first);
}

template <class RandomAccessIterator, class Compare>
std::pair<std::size_t, std::size_t>
merge_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	const auto size {std::distance(first, last)};
	std::size_t m {}, n {};
	for (std::size_t i {1}; i < size; i *= 2) {
		for (auto _first {first}, _last {std::next(_first, i * 2)}; 
			std::next(_first, i) < last; _first = _last, std::advance(_last, i * 2)) {
			if (_last > last)
				_last = last;
			auto mid {std::next(_first, i)};
			merge(_first, mid, _last, comp, m, n);
		}			
		std::cout << i * 2 << ": ";
		std::copy(first, last, std::ostream_iterator<int>{std::cout, "\t"});
		std::cout << '\n';
	}
	return {m, n};
}

int main()
{	
	std::string s; std::getline(std::cin, s);
	std::istringstream is {s};
	std::istream_iterator<int> begin {is}, end;
	std::vector<int> v(begin, end);
	const auto [m, n] {merge_sort(std::begin(v), std::end(v), std::less<int>{})};
	std::cout << "m = " << m << '\n';
	std::cout << "n = " << n << '\n';
	std::cout << "m + n = " << m + n << '\n';
	return EXIT_SUCCESS;
}
