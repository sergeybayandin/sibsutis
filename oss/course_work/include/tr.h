#ifndef __TR_H__
#define __TR_H__

#include <bit>
#include <utility>
#include <limits>

namespace os
{
	
	namespace details
	{

		template <class _Tp, std::size_t ...I>
		constexpr _Tp byteswap_impl(_Tp n, std::index_sequence<I...>) noexcept
		{
			constexpr auto digits {
				std::numeric_limits<unsigned char>::digits
			};
			return ((((n >> (I * digits)) & (0xff)) << (sizeof(_Tp) - I - 1) * digits) | ...);
		}

		template <class _Tp>
		constexpr _Tp byteswap(_Tp n) noexcept
		{
			return 
				byteswap_impl(
					n, 
					std::make_index_sequence<sizeof(_Tp)>{}
				);
		}

	} // details

	template <class _Tp>
	struct htons
	{
		static constexpr auto value {
			std::endian::native == std::endian::big
			? _Tp::value
			: details::byteswap(_Tp::value)
		};
	};

	struct <class _Tp, char sep = '.', std::size_t buff_size = 19>
	struct inet_ntoa
	{
		
	private:
		static char buffer[buff_size];
	};

} // os

#endif // __TR_H__
