#ifndef __TR_H__
#define __TR_H__

#include <bit>
#include <utility>
#include <limits>
#include <cstdio>

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
	constexpr _Tp hton(_Tp n)
	{
		return 
			std::endian::native == std::endian::big
			? n
			: details::byteswap(n);
	}

	template <class _Tp>
	const char *inet_ntoa(_Tp n)
	{
		static char buff[45];
		auto buff_first {buff};
		auto
			n_first {reinterpret_cast<unsigned char*>(&n)},
			n_last {n_first + sizeof(_Tp)};

		for (; n_first < n_last; ++n_first) {
			buff_first +=
				sprintf(buff_first, "%u.", static_cast<unsigned>(*n_first));
		}
		*(buff_first - 1) = '\0';

		return buff;
	}

} // os

#endif // __TR_H__
