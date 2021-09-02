#define _USE_MATH_DEFINES

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <complex>
#include <cmath>

template <class ForwardIterator, class OutputIterator>
std::size_t dpf(ForwardIterator first, ForwardIterator last,
	OutputIterator result)
{
	using complex_type = std::remove_reference_t<decltype(*first)>;
	using value_type   = typename complex_type::value_type;
	std::size_t n_dpf {};
	const auto n {std::distance(first, last)};
	const auto _2pii {2 * M_PI * complex_type({}, -1)};
	for (std::size_t k {}; k < n; ++k, ++result) {
		auto _first {first};
		complex_type sum;
		for (std::size_t j {}; j < n; ++j, ++_first, n_dpf += 5)
			sum += *_first * std::exp(_2pii * ((static_cast<value_type>(k) * j) / n));
		*result = sum / static_cast<value_type>(n); n_dpf += 2;
	}
	return n_dpf;
}

template <class ForwardIterator, class OutputIterator>
std::size_t odpf(ForwardIterator first, ForwardIterator last,
	OutputIterator result)
{
	using complex_type = std::remove_reference_t<decltype(*first)>;
	using value_type   = typename complex_type::value_type;
	std::size_t n_odpf {};
	const auto n {std::distance(first, last)};
	const auto _2pii {2 * M_PI * complex_type({}, -1)};
	for (std::size_t k {}; k < n; ++k, ++result) {
		auto _first {first};
		complex_type sum;
		for (std::size_t j {}; j < n; ++j, ++_first, n_odpf += 5)
			sum += *_first * std::exp(_2pii * ((static_cast<value_type>(k) * j) / n));
		*result = sum; ++n_odpf;
	}
	return n_odpf;
}

int main()
{
	std::string s; std::getline(std::cin, s);
	std::istringstream is {s};
	std::istream_iterator<std::complex<double>> begin {is}, end;
	std::vector<std::complex<double>> v(begin, end);
	const auto n_dpf {dpf(std::begin(v), std::end(v), std::begin(v))};
	for (auto val : v)
		std::cout << val << '\t';
	std::cout << "\noperations: " << n_dpf << '\n';
	const auto n_odpf {odpf(std::begin(v), std::end(v), std::begin(v))};
	for (auto val : v)
		std::cout << val << '\t';
	std::cout << "\noperations: " << n_odpf << '\n';
	return EXIT_SUCCESS;
}
