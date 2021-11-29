#include "../include/fast_multiplication.h"

namespace rgz
{

	namespace details
	{

    std::size_t
    count_buffer_size(std::size_t n)
    {
      std::size_t buffer_size {};

      for (; n; n /= 2) {
        buffer_size += n * n;
      }

      return 9 * buffer_size;
    }

	} // details

	std::pair<std::size_t, std::size_t>
	fast_multiplication(
		const std::vector<int> &a,
		const std::vector<int> &b,
		std::vector<int> &c,
		std::size_t n
	)
	{
		std::size_t multiplications {}, additions {};
		std::vector<int> buffer(
			details::count_buffer_size(n / 2)
		);
		
		details::fast_multiplication_impl(
			a.begin(),
			b.begin(),
			c.begin(),
			buffer.begin(),
			n,
			multiplications,
      additions
		);

		return {multiplications, additions};
	}

} // rgz
