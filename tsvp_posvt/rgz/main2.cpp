#include "include/fast_multiplication.h"
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

	decltype(n) n2 {1};

	while (n2 < n) {
		n2 *= 2;
	}

	std::vector<int> a(n2 * n2), b(n2 * n2), c(n2 * n2);
  
	for (std::size_t i {}; i < n; ++i) {
    for (std::size_t j {}; j < n; ++j) {
      a[i * n2 + j] = ((i + j) % 2 ? -1 : 1);
    }
  }

  for (std::size_t i {}; i < n; ++i) {
    for (std::size_t j {}; j < n; ++j) {
      b[i * n2 + j] = i + j;
    }
  }

	auto [multiplications, additions] {
		rgz::fast_multiplication(a, b, c, n2)
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
