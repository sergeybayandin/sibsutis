#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	using size_type = typename std::string::size_type;
	std::string snum1, snum2; std::cin >> snum1 >> snum2;
	auto is_num1_negative {snum1.front() == '-' ? size_type{1} : 0}, 
			is_num2_negative {snum2.front() == '-' ? size_type{1} : 0};
	std::vector<short> num1(snum1.size() - is_num1_negative), 
			num2(snum2.size() - is_num2_negative);
	std::transform(std::next(std::begin(snum1), is_num1_negative), std::end(snum1), 
		std::rbegin(num1), [](auto ch){return ch - '0';});
	std::transform(std::next(std::begin(snum2), is_num2_negative), std::end(snum2), 
		std::rbegin(num2), [](auto ch){return ch - '0';});
	std::vector<short> result; result.reserve(num1.size() + num2.size());
	for (typename std::vector<short>::size_type i {}; i < num1.size(); ++i) {
		for (typename std::vector<short>::size_type j {}; j < num2.size(); ++j) {
			if (i + j >= result.size())
				result.push_back(num1[i] * num2[j]);
			else
				result[i + j] += num1[i] * num2[j];
		}
	}
	for (typename std::vector<short>::size_type i {}; i < result.size(); ++i) {
		if (result[i] > 9) {
			if (i + 1 >= result.size())
				result.push_back(result[i] / 10);
			else
				result[i + 1] += result[i] / 10;
			result[i] %= 10;
		}
	}
	if (is_num1_negative + is_num2_negative == 1) {
		std::cout << '-';
	}
	for (typename std::vector<short>::size_type i {}; i < result.size(); ++i)
		std::cout << result[result.size() - i - 1];
	std::cout << '\n';
	return EXIT_SUCCESS;
}
