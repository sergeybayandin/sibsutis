#ifndef __FAST_MULTIPLICATION_H__
#define __FAST_MULTIPLICATION_H__

#include <vector>

/*


	0 0  0 0
	0 0  0 0
	
	0 0  0 0 
	0 0  0 0 

*/


namespace rgz
{

	namespace details
	{
		
		using vector2d = std::vector<int>;
		using size_type = vector2d::size_type;

		template <class OutputIt, class InputIt>
		void count_c1(
			OutputIt first_it, 
			InputIt x1_it, 
			InputIt x2_it,
			InputIt x3_it,
			InputIt x4_it,
			size_type n2
		)
		{
			auto last_it {first_it + 2 * n2 * n2 - n2};
			for (; first_it < last_it; first_it += n2) {
				auto last1_it {first_it + n2};
				for (; first_it < last1_it; ++first_it) {
					*first_it = *x1_it + *x2_it - *x3_it + *x4_it;
					++x1_it; 
					++x2_it; 
					++x3_it;
					++x4_it;
				}
			}
		}
		
		template <class OutputIt, class InputIt>
		void count_c2c3(
			OutputIt first_it,
			InputIt x1_it,
			InputIt x2_it,
			size_type n2
		)
		{
			auto last_it {first_it + 2 * n2 * n2 - n2};
			for (; first_it < last_it; first_it += n2) {
				auto last1_it {first_it + n2};
				for (; first_it < last1_it; ++first_it) {
					*first_it = *x1_it + *x2_it;
					++x1_it; 
					++x2_it;
				}
			}
		}

		template <class OutputIt, class InputIt>
		void split_4_into_1(
			OutputIt first_it,
			InputIt x1_it,
			InputIt x2_it,
			InputIt x3_it,
			InputIt x4_it,
			size_type n2
		)
		{
			OutputIt last_it {first_it + 2 * n2 * n2}, last1_it;
			for (; first_it < last_it; ) {
				last1_it = first_it + n2;
				for (; first_it < last1_it; ++first_it, ++x1_it) {
					*first_it = *x1_it;
				}				
				last1_it = first_it + n2;
				for (; first_it < last1_it; ++first_it, ++x2_it) {
					*first_it = *x2_it;
				}
			}
			last_it = first_it + 2 * n2 * n2;			
			for (; first_it < last_it; ) {
				last1_it = first_it + n2;
				for (; first_it < last1_it; ++first_it, ++x3_it) {
					*first_it = *x3_it;
				}				
				last1_it = first_it + n2;
				for (; first_it < last1_it; ++first_it, ++x4_it) {
					*first_it = *x4_it;
				}
			}
		}

		template <class OutputIt, class InputIt>
		void count_c4(
			OutputIt first_it, 
			InputIt x1_it, 
			InputIt x2_it,
			InputIt x3_it,
			InputIt x4_it,
			size_type n2
		)
		{
			auto last_it {first_it + 2 * n2 * n2 - n2};
			for (; first_it < last_it; first_it += n2) {
				auto last1_it {first_it + n2};
				for (; first_it < last1_it; ++first_it) {
					*first_it = *x1_it - *x2_it + *x3_it - *x4_it;
					++x1_it; 
					++x2_it; 
					++x3_it;
					++x4_it;
				}
			}
		}	

		template <class OutputIt, class InputIt>
		OutputIt
		copy_buffer(
			OutputIt buffer_it, 
			InputIt first_it, 
			InputIt last_it, 
			size_type n)
		{
			InputIt last_it1;
			for (; first_it < last_it; first_it += n) {
				last_it1 = first_it + n;
				for (; first_it < last_it1; ++first_it, ++buffer_it) {
					*buffer_it = *first_it;
				}
			}
			return buffer_it;
		}

		void fast_multiplication_impl(
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			vector2d::iterator,
			size_type,
			vector2d::iterator,
			std::size_t&,
			std::size_t&
		);

	} // details

	std::pair<std::size_t, std::size_t>
	fast_multiplication(
		const details::vector2d&,
		const details::vector2d&,
		details::vector2d&,
		details::size_type
	);

} // rgz

#endif // __FAST_MULTIPLICATION_H__
