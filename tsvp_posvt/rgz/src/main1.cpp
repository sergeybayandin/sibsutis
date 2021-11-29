#include "../include/slow_multiplication.h"
#include <cstdlib>
#include <cerrno>
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr
			<< (argc > 2 ? "Too many arguments\n" : "Arguments omitted\n");
		return EXIT_FAILURE;
	}
	
	char* str_end;
	auto n {std::strtoull(argv[1], &str_end, 10)};
	
	if (!n || errno == ERANGE || *str_end) {
		std::cerr << "Wrong " << argv[1] << '\n';
		return EXIT_FAILURE;
	}

	std::vector<int> a(n * n), b(n * n), c(n * n);

	for (std::size_t i {}; i < n; ++i) {
		for (std::size_t j {}; j < n; ++j) {
			a[i * n + j] = ((i + j) % 2 ? -1 : 1);
		}
	}
	
	for (std::size_t i {}; i < n; ++i) {
		for (std::size_t j {}; j < n; ++j) {
			b[i * n + j] = i + j;
		}
	}

	auto [multiplications, additions] {
		rgz::slow_multiplication(a, b, c, n)
	};

	std::cout << "multiplications: " << multiplications << '\n';
	std::cout << "additions: " << additions << '\n';
	std::cout 
		<< "T(n) = " 
		<< multiplications 
		<< " + " 
		<< additions
		<< " = "
		<< multiplications + additions
		<< '\n';

	return EXIT_SUCCESS;
}
