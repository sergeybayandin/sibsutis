#ifndef __SOCKIOS_H__
#define __SOCKIOS_H__

#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/in.h>
#include <cstring>

namespace os
{
	
	template <class _So>
	auto get_ip(const _So &sock, const char *if_name, bool *ok = {}) noexcept
	{
		ifreq ifr; 

		std::strcpy(ifr.ifr_name, if_name);
		const auto res {
			::ioctl(sock.descriptor(), SIOCGIFADDR, &ifr) >= 0
		};
		const auto s_addr {
			reinterpret_cast<sockaddr_in*>(&ifr.ifr_addr)
		};
		if (ok) {
			*ok = res;
		}

		return s_addr->sin_addr;
	}

	template <class _So>
	auto get_netmask(const _So &sock, const char *if_name, bool *ok = {}) noexcept
	{
		ifreq ifr; 

		std::strcpy(ifr.ifr_name, if_name);
		const auto res {
			::ioctl(sock.descriptor(), SIOCGIFNETMASK, &ifr) >= 0
		};
		const auto s_addr {
			reinterpret_cast<sockaddr_in*>(&ifr.ifr_netmask)
		};
		if (ok) {
			*ok = res;
		}

		return s_addr->sin_addr;
	}

	template <class _So>
	auto get_mtu(const _So &sock, const char *if_name, bool *ok = {}) noexcept
	{
		ifreq ifr; 

		std::strcpy(ifr.ifr_name, if_name);
		const auto res {
			::ioctl(sock.descriptor(), SIOCGIFMTU, &ifr) >= 0
		};
		if (ok) {
			*ok = res;
		}

		return ifr.ifr_mtu;
	}
	
	template <class _So>
	auto get_ifindex(const _So &sock, const char *if_name, bool *ok = {}) noexcept
	{
		ifreq ifr; 

		std::strcpy(ifr.ifr_name, if_name);
		const auto res {
			::ioctl(sock.descriptor(), SIOCGIFINDEX, &ifr) >= 0
		};
		if (ok) {
			*ok = res;
		}

		return ifr.ifr_ifindex;
	}

	template <class _So>
	auto get_flags(const _So &sock, const char *if_name, bool *ok = {}) noexcept
	{
		ifreq ifr; 

		std::strcpy(ifr.ifr_name, if_name);
		const auto res {
			::ioctl(sock.descriptor(), SIOCGIFFLAGS, &ifr) >= 0
		};
		if (ok) {
			*ok = res;
		}

		return ifr.ifr_flags;
	}
	
	template <bool mode, class _So, class _Flag>
	bool change_flags(const _So &sock, const char *if_name, _Flag flag) noexcept
	{
		bool ok;
		
		auto curr_flags {
			get_flags(sock, if_name, &ok)
		};

		if (!ok) {
			return false;
		}

		ifreq ifr; 

		std::strcpy(ifr.ifr_name, if_name);

		if constexpr (mode) {
			ifr.ifr_flags = curr_flags | flag;
		} else {
			ifr.ifr_flags = curr_flags & (~flag);
		}
		
		return ::ioctl(sock.descriptor(), SIOCSIFFLAGS, &ifr) >= 0;
	}
	
} // os

#endif // __SOCKIOS_H__
