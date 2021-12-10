#ifndef __TR_H__
#define __TR_H__

#include <bit>
#include <utility>
#include <limits>
#include <cstdio>
#include <linux/if_ether.h>

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

	static char _buff[18];

	template <class _Tp>
	const char *inet_ntoa(_Tp n)
	{
		constexpr auto digits {
			std::numeric_limits<unsigned char>::digits
		};
		sprintf(
			_buff,
			"%u.%u.%u.%u\0",
			n & 0xff, (n >> digits) & 0xff, 
			(n >> 2 * digits) & 0xff, (n >> 3 * digits) & 0xff
		);
		return _buff;
	}

	template <class _Tp>
	const char *eth_ntoa(const _Tp n[ETH_ALEN])
	{
		sprintf(
			_buff,
			"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\0",
			n[0], n[1], n[2], n[3], n[4], n[5]
		);
		return _buff;
	}

} // os

#endif // __TR_H__
