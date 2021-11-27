#include "../include/fast_multiplication.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace rgz
{

	namespace details
	{
		void fast_multiplication_impl(
			vector2d::iterator a1_it,
			vector2d::iterator a2_it,
			vector2d::iterator a3_it,
			vector2d::iterator a4_it,
			vector2d::iterator b1_it,
			vector2d::iterator b2_it,
			vector2d::iterator b3_it,
			vector2d::iterator b4_it,
			vector2d::iterator c1_it,
			vector2d::iterator c2_it,
			vector2d::iterator c3_it,
			vector2d::iterator c4_it,
			size_type n,
			vector2d::iterator buff_it,
			std::size_t& multiplications,
			std::size_t& additions
		)
		{
			if (n > 1) {
				const auto nn {n * n}, n2 {n / 2}, n2n2 {n2 * n2}, offset {2 * n2n2};
				auto temp1_it {buff_it};
				
				buff_it += nn;

				std::transform(a2_it, a2_it + nn, a4_it, temp1_it, std::minus<int>{});
			
				auto temp2_it {buff_it};	
		
				buff_it += nn;
	
				std::transform(b3_it, b3_it + nn, b4_it, temp2_it, std::plus<int>{});

				auto x1_it {buff_it};
				
				auto x2_it {
					copy_buffer(
						x1_it, 
						temp1_it,
						temp1_it + offset - n2, 
						n2
					)
				};

				auto x3_it {
					copy_buffer(
						x2_it,
						temp1_it + n2, 
						temp1_it + offset, 
						n2
					)
				};

				auto x4_it {
					copy_buffer(
						x3_it,
						temp1_it + offset, 
						temp1_it + 2 * offset - n2,
						n2
					)
				};
				
				buff_it = 
					copy_buffer(
						x4_it,
						temp1_it + offset + n2, 
						temp1_it + 2 * offset,
						n2
					);
				
				auto y1_it {buff_it};
				
				auto y2_it {
					copy_buffer(
						y1_it, 
						temp2_it,
						temp2_it + offset - n2, 
						n2
					)
				};

				auto y3_it {
					copy_buffer(
						y2_it,
						temp2_it + n2, 
						temp2_it + offset, 
						n2
					)
				};

				auto y4_it {
					copy_buffer(
						y3_it,
						temp2_it + offset, 
						temp2_it + 2 * offset - n2,
						n2
					)
				};
				
				buff_it = 
					copy_buffer(
						y4_it,
						temp2_it + offset + n2, 
						temp2_it + 2 * offset,
						n2
					);
				
				auto 
					m11_it {buff_it},
					m12_it {m11_it + n2n2},
					m13_it {m12_it + n2n2},
					m14_it {m13_it + n2n2};

				buff_it = m14_it + n2n2;

				fast_multiplication_impl(
					x1_it,
					x2_it,
					x3_it,
					x4_it,
					y1_it,
					y2_it,
					y3_it,
					y4_it,
					m11_it,
					m12_it,
					m13_it,
					m14_it,
					n2,
					buff_it,
					multiplications,
					additions
				);
					
				std::transform(
					a1_it, 
					a1_it + nn, 
					a4_it, 
					temp1_it, 
					std::plus<int>{}
				);
				
				std::transform(
					b1_it, 
					b1_it + nn, 
					b4_it, 
					temp2_it, 
					std::plus<int>{}
				);

				copy_buffer(
					x1_it, 
					temp1_it,
					temp1_it + offset - n2, 
					n2
				);
				
				copy_buffer(
					x2_it,
					temp1_it + n2, 
					temp1_it + offset, 
					n2
				);

				copy_buffer(
					x3_it,
					temp1_it + offset, 
					temp1_it + 2 * offset - n2,
					n2
				);
				
				copy_buffer(
					x4_it,
					temp1_it + offset + n2, 
					temp1_it + 2 * offset,
					n2
				);
				
				copy_buffer(
					y1_it, 
					temp2_it,
					temp2_it + offset - n2, 
					n2
				);
		
				copy_buffer(
					y2_it,
					temp2_it + n2, 
					temp2_it + offset, 
					n2
				);
				
				copy_buffer(
					y3_it,
					temp2_it + offset, 
					temp2_it + 2 * offset - n2,
					n2
				);

				copy_buffer(
					y4_it,
					temp2_it + offset + n2, 
					temp2_it + 2 * offset,
					n2
				);

				auto 
					m21_it {buff_it},
					m22_it {m21_it + n2n2},
					m23_it {m22_it + n2n2},
					m24_it {m23_it + n2n2};

				buff_it = m24_it + n2n2;

				fast_multiplication_impl(
					x1_it,
					x2_it,
					x3_it,
					x4_it,
					y1_it,
					y2_it,
					y3_it,
					y4_it,
					m21_it,
					m22_it,
					m23_it,
					m24_it,
					n2,
					buff_it,
					multiplications,
					additions
				);

				std::transform(
					a1_it, 
					a1_it + nn, 
					a3_it, 
					temp1_it,
					std::minus<int>{}
				);

				std::transform(
					b1_it,
					b1_it + nn,
					b2_it,
					temp2_it,
					std::plus<int>{}
				);

				copy_buffer(
					x1_it, 
					temp1_it,
					temp1_it + offset - n2, 
					n2
				);
				
				copy_buffer(
					x2_it,
					temp1_it + n2, 
					temp1_it + offset, 
					n2
				);

				copy_buffer(
					x3_it,
					temp1_it + offset, 
					temp1_it + 2 * offset - n2,
					n2
				);
				
				copy_buffer(
					x4_it,
					temp1_it + offset + n2, 
					temp1_it + 2 * offset,
					n2
				);
				
				copy_buffer(
					y1_it, 
					temp2_it,
					temp2_it + offset - n2, 
					n2
				);
		
				copy_buffer(
					y2_it,
					temp2_it + n2, 
					temp2_it + offset, 
					n2
				);
				
				copy_buffer(
					y3_it,
					temp2_it + offset, 
					temp2_it + 2 * offset - n2,
					n2
				);

				copy_buffer(
					y4_it,
					temp2_it + offset + n2, 
					temp2_it + 2 * offset,
					n2
				);

				auto 
					m31_it {buff_it},
					m32_it {m31_it + n2n2},
					m33_it {m32_it + n2n2},
					m34_it {m33_it + n2n2};
			
				buff_it = m34_it + n2n2;
				
				fast_multiplication_impl(
					x1_it,
					x2_it,
					x3_it,
					x4_it,
					y1_it,
					y2_it,
					y3_it,
					y4_it,
					m31_it,
					m32_it,
					m33_it,
					m34_it,
					n2,
					buff_it,
					multiplications,
					additions
				);

				std::transform(
					a1_it, 
					a1_it + nn, 
					a2_it, 
					temp1_it, 
					std::plus<int>{}
				);

				copy_buffer(
					x1_it, 
					temp1_it,
					temp1_it + offset - n2, 
					n2
				);
				
				copy_buffer(
					x2_it,
					temp1_it + n2, 
					temp1_it + offset, 
					n2
				);

				copy_buffer(
					x3_it,
					temp1_it + offset, 
					temp1_it + 2 * offset - n2,
					n2
				);
				
				copy_buffer(
					x4_it,
					temp1_it + offset + n2, 
					temp1_it + 2 * offset,
					n2
				);			

				copy_buffer(
					y1_it, 
					b4_it,
					b4_it + offset - n2, 
					n2
				);
		
				copy_buffer(
					y2_it,
					b4_it + n2, 
					b4_it + offset, 
					n2
				);
				
				copy_buffer(
					y3_it,
					b4_it + offset, 
					b4_it + 2 * offset - n2,
					n2
				);

				copy_buffer(
					y4_it,
					b4_it + offset + n2, 
					b4_it + 2 * offset,
					n2
				);
				
				auto
					m41_it {buff_it},
					m42_it {m41_it + n2n2},
					m43_it {m42_it + n2n2},
					m44_it {m43_it + n2n2};
			
				buff_it = m44_it + n2n2;
				
				fast_multiplication_impl(
					x1_it,
					x2_it,
					x3_it,
					x4_it,
					y1_it,
					y2_it,
					y3_it,
					y4_it,
					m41_it,
					m42_it,
					m43_it,
					m44_it,
					n2,
					buff_it,
					multiplications,
					additions
				);
				
				std::transform(
					b2_it, 
					b2_it + nn, 
					b4_it, 
					temp1_it, 
					std::minus<int>{}
				);

				copy_buffer(
					x1_it, 
					a1_it,
					a1_it + offset - n2, 
					n2
				);
				
				copy_buffer(
					x2_it,
					a1_it + n2, 
					a1_it + offset, 
					n2
				);

				copy_buffer(
					x3_it,
					a1_it + offset, 
					a1_it + 2 * offset - n2,
					n2
				);
				
				copy_buffer(
					x4_it,
					a1_it + offset + n2, 
					a1_it + 2 * offset,
					n2
				);				

				copy_buffer(
					y1_it, 
					temp1_it,
					temp1_it + offset - n2, 
					n2
				);
		
				copy_buffer(
					y2_it,
					temp1_it + n2, 
					temp1_it + offset, 
					n2
				);
				
				copy_buffer(
					y3_it,
					temp1_it + offset, 
					temp1_it + 2 * offset - n2,
					n2
				);

				copy_buffer(
					y4_it,
					temp1_it + offset + n2, 
					temp1_it + 2 * offset,
					n2
				);

				auto 
					m51_it {buff_it},
					m52_it {m51_it + n2n2},
					m53_it {m52_it + n2n2},
					m54_it {m53_it + n2n2};

				buff_it = m54_it + n2n2;

				fast_multiplication_impl(
					x1_it,
					x2_it,
					x3_it,
					x4_it,
					y1_it,
					y2_it,
					y3_it,
					y4_it,
					m51_it,
					m52_it,
					m53_it,
					m54_it,
					n2,
					buff_it,
					multiplications,
					additions
				);

				std::transform(
					b3_it,
					b3_it + nn,
					b1_it,
					temp1_it,
					std::minus<int>{}
				);

				copy_buffer(
					x1_it, 
					a4_it,
					a4_it + offset - n2, 
					n2
				);
				
				copy_buffer(
					x2_it,
					a4_it + n2, 
					a4_it + offset, 
					n2
				);

				copy_buffer(
					x3_it,
					a4_it + offset, 
					a4_it + 2 * offset - n2,
					n2
				);
				
				copy_buffer(
					x4_it,
					a4_it + offset + n2, 
					a4_it + 2 * offset,
					n2
				);				

				copy_buffer(
					y1_it, 
					temp1_it,
					temp1_it + offset - n2, 
					n2
				);
		
				copy_buffer(
					y2_it,
					temp1_it + n2, 
					temp1_it + offset, 
					n2
				);
				
				copy_buffer(
					y3_it,
					temp1_it + offset, 
					temp1_it + 2 * offset - n2,
					n2
				);

				copy_buffer(
					y4_it,
					temp1_it + offset + n2, 
					temp1_it + 2 * offset,
					n2
				);

				auto
					m61_it {buff_it},
					m62_it {m61_it + n2n2},
					m63_it {m62_it + n2n2},
					m64_it {m63_it + n2n2};
		
				buff_it = m64_it + n2n2;

				fast_multiplication_impl(
					x1_it,
					x2_it,
					x3_it,
					x4_it,
					y1_it,
					y2_it,
					y3_it,
					y4_it,
					m61_it,
					m62_it,
					m63_it,
					m64_it,
					n2,
					buff_it,
					multiplications,
					additions
				);

				std::transform(a3_it, a3_it + nn, a4_it, temp1_it, std::plus<int>{});

				copy_buffer(
					x1_it, 
					temp1_it,
					temp1_it + offset - n2, 
					n2
				);
				
				copy_buffer(
					x2_it,
					temp1_it + n2, 
					temp1_it + offset, 
					n2
				);

				copy_buffer(
					x3_it,
					temp1_it + offset, 
					temp1_it + 2 * offset - n2,
					n2
				);
				
				copy_buffer(
					x4_it,
					temp1_it + offset + n2, 
					temp1_it + 2 * offset,
					n2
				);	
				
				copy_buffer(
					y1_it, 
					b1_it,
					b1_it + offset - n2, 
					n2
				);
		
				copy_buffer(
					y2_it,
					b1_it + n2, 
					b1_it + offset, 
					n2
				);
				
				copy_buffer(
					y3_it,
					b1_it + offset, 
					b1_it + 2 * offset - n2,
					n2
				);

				copy_buffer(
					y4_it,
					b1_it + offset + n2, 
					b1_it + 2 * offset,
					n2
				);

				auto
					m71_it {buff_it},
					m72_it {m71_it + n2n2},
					m73_it {m72_it + n2n2},
					m74_it {m73_it + n2n2};
				
				buff_it = m74_it + n2n2;

				fast_multiplication_impl(
					x1_it,
					x2_it,
					x3_it,
					x4_it,
					y1_it,
					y2_it,
					y3_it,
					y4_it,
					m71_it,
					m72_it,
					m73_it,
					m74_it,
					n2,
					buff_it,
					multiplications,
					additions
				);
			
				count_c1(
					c1_it, 
					m11_it, 
					m21_it, 
					m41_it,
					m61_it,
					n2
				);
				
				count_c1(
					c1_it + n2,
					m12_it,
					m22_it,
					m42_it,
					m62_it,
					n2
				);				
				
				count_c1(
					c1_it + offset,
					m13_it,
					m23_it,
					m43_it,
					m63_it,
					n2
				);
				
				count_c1(
					c1_it + offset + n2,
					m14_it,
					m24_it,
					m44_it,
					m64_it,
					n2
				);
				
				count_c2c3(
					c2_it, 
					m41_it, 
					m51_it, 
					n2
				);
				
				count_c2c3(
					c2_it + n2,
					m42_it,
					m52_it,
					n2
				);				
				
				count_c2c3(
					c2_it + offset,
					m43_it,
					m53_it,
					n2
				);
				
				count_c2c3(
					c2_it + offset + n2,
					m44_it,
					m54_it,
					n2
				);		
				
				count_c2c3(
					c3_it, 
					m61_it, 
					m71_it, 
					n2
				);
				
				count_c2c3(
					c3_it + n2,
					m62_it,
					m72_it,
					n2
				);				
				
				count_c2c3(
					c3_it + offset,
					m63_it,
					m73_it,
					n2
				);
				
				count_c2c3(
					c3_it + offset + n2,
					m64_it,
					m74_it,
					n2
				);
			
				count_c4(
					c4_it, 
					m21_it, 
					m31_it, 
					m51_it,
					m71_it,
					n2
				);
				
				count_c4(
					c4_it + n2,
					m22_it,
					m32_it,
					m52_it,
					m72_it,
					n2
				);				
				
				count_c4(
					c4_it + offset,
					m23_it,
					m33_it,
					m53_it,
					m73_it,
					n2
				);
				
				count_c4(
					c4_it + offset + n2,
					m24_it,
					m34_it,
					m54_it,
					m74_it,
					n2
				);
			} else {
				*c1_it = *a1_it * *b1_it + *a2_it * *b3_it;
				*c2_it = *a1_it * *b2_it + *a2_it * *b4_it;
				*c3_it = *a3_it * *b1_it + *a4_it * *b3_it;
				*c4_it = *a3_it * *b2_it + *a4_it * *b4_it;
				multiplications += 8;
				additions += 4;
			}
		}
	
	} // details

	std::pair<std::size_t, std::size_t>
	fast_multiplication(
		const details::vector2d &a,
		const details::vector2d &b,
		details::vector2d &c,
		details::size_type n
	)
	{
		const auto n2 {n / 2}, offset {2 * n2 * n2};
		std::size_t multiplications {}, additions {};
		details::vector2d buffer(
			12 * n * n + n2 * n2,
			{}
		);

		auto a1_it {buffer.begin()};

		auto a2_it {
			details::copy_buffer(
				a1_it, 
				a.begin(),
				a.begin() + offset - n2, 
				n2
			)
		};	
		
		auto a3_it {
			details::copy_buffer(
				a2_it,
				a.begin() + n2, 
				a.begin() + offset, 
				n2
			)
		};
		
		auto a4_it {
			details::copy_buffer(
				a3_it,
				a.begin() + offset, 
				a.begin() + 2 * offset - n2,
				n2
			)
		};
		
		auto b1_it {
			details::copy_buffer(
				a4_it,
				a.begin() + offset + n2, 
				a.end(),
				n2
			)
		};

		auto b2_it {
			details::copy_buffer(
				b1_it,
				b.begin(), 
				b.begin() + offset - n2,
				n2
			)
		};
		
		auto b3_it {
			details::copy_buffer(
				b2_it,
				b.begin() + n2, 
				b.begin() + offset, 
				n2
			)
		};
		
		auto b4_it {
			details::copy_buffer(
				b3_it,
				b.begin() + offset, 
				b.begin() + 2 * offset - n2,
				n2
			)
		};
		
		auto c1_it {
			details::copy_buffer(
				b4_it,
				b.begin() + offset + n2, 
				b.end(),
				n2
			)
		};

		auto 
			c2_it {c1_it + n2 * n2},
			c3_it {c2_it + n2 * n2},
			c4_it {c3_it + n2 * n2},
			buff_it {c4_it + n2 * n2};

		details::fast_multiplication_impl(
			a1_it,
			a2_it,
			a3_it,
			a4_it,
			b1_it,
			b2_it,
			b3_it,
			b4_it,	
			c1_it,
			c2_it,
			c3_it,
			c4_it,
			n2,
			buff_it,
			multiplications,
			additions
		);
			
		details::split_4_into_1(
			c.begin(),
			c1_it,
			c2_it,
			c3_it,
			c4_it,
			n2
		);	

		return {multiplications, additions};
	}

} // rgz
