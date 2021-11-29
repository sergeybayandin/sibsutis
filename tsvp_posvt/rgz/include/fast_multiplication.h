#ifndef __FAST_MULTIPLICATION_H__
#define __FAST_MULTIPLICATION_H__

#include <vector>
#include <utility>
#include <algorithm>

namespace rgz
{
	
	namespace details
	{
	
		std::size_t 
		count_buffer_size(std::size_t);

		template <class AIt, class BIt, class CIt, class Operation>
		void sub_or_add_n2xn2(
			AIt a_it,
			BIt b_it,
			CIt first_it,
			CIt last_it,
			std::size_t n2,
			Operation op
		)
		{
			CIt last1_it;
			for (; first_it < last_it; ) {
				for (last1_it = first_it + n2; first_it < last1_it; ) {
					*first_it = op(*a_it, *b_it);
					++first_it;
					++a_it,
					++b_it;
				}
				a_it += n2;
				b_it += n2;
			}
		}

		template <class AIt, class BIt>
		void assign_n2xn2(
			AIt a_it, 
			BIt first_it, 
			BIt last_it, 
			std::size_t n2
		)
		{
			BIt last1_it;
			for (; first_it < last_it; ) {
				for (last1_it = first_it + n2; first_it < last1_it; ) {
					*first_it = *a_it;
					++first_it;
					++a_it;
				}
				a_it += n2;
			}
		}

		template <class CIt, class MIt, class Operation>
		void c1_or_c4_formula_n2xn2(
			MIt m1_it,
			MIt m2_it,
			MIt m3_it,
			MIt m4_it,
			CIt first_it,
			CIt last_it,
			std::size_t n2,
			Operation op
		)
		{
			CIt last1_it;
			for (; first_it < last_it; first_it += n2) {
				for (last1_it = first_it + n2; first_it < last1_it; ++first_it) {
					*first_it = op(*m1_it, *m2_it, *m3_it, *m4_it);
					++m1_it;
					++m2_it;
					++m3_it;
					++m4_it;
				}
			}
		}

		template <class CIt, class MIt, class Operation>
		void c2_or_c3_formula_n2xn2(
			MIt m1_it,
			MIt m2_it,
			CIt first_it,
			CIt last_it,
			std::size_t n2,
			Operation op
		)
		{
			CIt last1_it;
			for (; first_it < last_it; first_it += n2) {
				for (last1_it = first_it + n2; first_it < last1_it; ++first_it) {
					*first_it = op(*m1_it, *m2_it);
					++m1_it;
					++m2_it;
				}
			}
		}

		template <class AIt, class BIt, class CIt, class BuffIt>
		void fast_multiplication_impl(
			AIt a_it,
			BIt b_it,
			CIt c_it,
			BuffIt buff_it,
			std::size_t n,
			std::size_t &multiplications,
			std::size_t &additions
		)
		{
			if (n > 1) {
				const auto 
					n2 			{n / 2}, 
					n2xn2   {n2 * n2}, 
					offset1 {2 * n2xn2},
					offset2 {offset1 + n2};
				
				auto temp1_it {buff_it}, temp2_it {buff_it + n2xn2};
				
				buff_it += 2 * n2xn2;

				sub_or_add_n2xn2(
					a_it + n2,
					a_it + offset2,
					temp1_it,
					temp1_it + n2xn2,
					n2,
					std::minus<int>{}
				);

				sub_or_add_n2xn2(
					b_it + offset1,
					b_it + offset2,
					temp2_it,
					temp2_it + n2xn2,
					n2,
					std::plus<int>{}
				);

				auto m1_it {buff_it};
	
				buff_it += n2xn2;

				fast_multiplication_impl(
					temp1_it,
					temp2_it,
					m1_it,
					buff_it,
					n2,
					multiplications,
					additions
				);

				sub_or_add_n2xn2(
					a_it,
					a_it + offset2,
					temp1_it,
					temp1_it + n2xn2,
					n2,
					std::plus<int>{}
				);

				sub_or_add_n2xn2(
					b_it,
					b_it + offset2,
					temp2_it,
					temp2_it + n2xn2,
					n2,
					std::plus<int>{}
				);

				auto m2_it {buff_it};
			
				buff_it += n2xn2;
			
				fast_multiplication_impl(
					temp1_it,
					temp2_it,
					m2_it,
					buff_it,
					n2,
					multiplications,
					additions
				);

				sub_or_add_n2xn2(
					a_it,
					a_it + offset1,
					temp1_it,
					temp1_it + n2xn2,
					n2,
					std::minus<int>{}
				);

				sub_or_add_n2xn2(
					b_it,
					b_it + n2,
					temp2_it,
					temp2_it + n2xn2,
					n2,
					std::plus<int>{}
				);

				auto m3_it {buff_it};

				buff_it += n2xn2;
				
				fast_multiplication_impl(
					temp1_it,
					temp2_it,
					m3_it,
					buff_it,
					n2,
					multiplications,
					additions
				);
			
				sub_or_add_n2xn2(
					a_it,
					a_it + n2,
					temp1_it,
					temp1_it + n2xn2,
					n2,
					std::plus<int>{}
				);

				assign_n2xn2(
					b_it + offset2,
					temp2_it,
					temp2_it + n2xn2,
					n2
				);

				auto m4_it {buff_it};
		
				buff_it += n2xn2;

				fast_multiplication_impl(
					temp1_it,
					temp2_it,
					m4_it,
					buff_it,
					n2,
					multiplications,
					additions
				);

				assign_n2xn2(
					a_it,
					temp1_it,
					temp1_it + n2xn2,
					n2
				);
				
				sub_or_add_n2xn2(
					b_it + n2,
					b_it + offset2,
					temp2_it,
					temp2_it + n2xn2,
					n2,
					std::minus<int>{}
				);
				
				auto m5_it {buff_it};

				buff_it += n2xn2;

				fast_multiplication_impl(
					temp1_it,
					temp2_it,
					m5_it,
					buff_it,
					n2,
					multiplications,
					additions
				);
			
				assign_n2xn2(
					a_it + offset2,
					temp1_it,
					temp1_it + n2xn2,
					n2
				);
				
				sub_or_add_n2xn2(
					b_it + offset1,
					b_it,
					temp2_it,
					temp2_it + n2xn2,
					n2,
					std::minus<int>{}
				);
				
				auto m6_it {buff_it};

				buff_it += n2xn2;

				fast_multiplication_impl(
					temp1_it,
					temp2_it,
					m6_it,
					buff_it,
					n2,
					multiplications,
					additions
				);

				sub_or_add_n2xn2(
					a_it + offset1,
					a_it + offset2,
					temp1_it,
					temp1_it + n2xn2,
					n2,
					std::plus<int>{}
				);

				assign_n2xn2(
					b_it,
					temp2_it,
					temp2_it + n2xn2,
					n2
				);			

				auto m7_it {buff_it};

				buff_it += n2xn2;

				fast_multiplication_impl(
					temp1_it,
					temp2_it,
					m7_it,
					buff_it,
					n2,
					multiplications,
					additions
				);

				c1_or_c4_formula_n2xn2(
					m1_it,
					m2_it,
					m4_it,
					m6_it,
					c_it,
					c_it + offset1 - n2,
					n2,
					[](int m1, int m2, int m4, int m6) {
						return m1 + m2 - m4 + m6;
					}
				);
		
				c1_or_c4_formula_n2xn2(
					m2_it,
					m3_it,
					m5_it,
					m7_it,
					c_it + offset2,
					c_it + n * n,
					n2,
					[](int m2, int m3, int m5, int m7) {
						return m2 - m3 + m5 - m7;
					}
				);

				c2_or_c3_formula_n2xn2(
					m4_it,	
					m5_it,
					c_it + n2,
					c_it + offset1,
					n2,
					[](int m5, int m6) {
						return m5 + m6;
					}
				);
				
				c2_or_c3_formula_n2xn2(
					m6_it,	
					m7_it,
					c_it + offset1,
					c_it + n * n - n2,
					n2,
					[](int m6, int m7) {
						return m6 + m7;
					}
				);
				additions += 18;
			} else {
				*c_it = *a_it * *b_it;
				++multiplications;
			}
		}
		
	} // details

	std::pair<std::size_t, std::size_t>
	fast_multiplication(
		const std::vector<int>&,
		const std::vector<int>&,
		std::vector<int>&,
		std::size_t
	);

} // rgz


#endif // __FAST_MULTIPLICATION_H__
