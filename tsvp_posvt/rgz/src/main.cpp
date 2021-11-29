#include "../include/fast_multiplication.h"
#include <vector>
#include <iostream>

int main()
{
	std::size_t n; std::cin >> n;
	std::vector<int> a(n * n), b(n * n), c(n * n);
	rgz::fast_multiplication(a, b, c, n);
	return 0;
}
