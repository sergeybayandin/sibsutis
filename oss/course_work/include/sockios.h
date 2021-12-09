#ifndef __SOCKIOS_H__
#define __SOCKIOS_H__

#include <linux/sockios.h>
#include <linux/ioctl.h>
#include <linux/if.h>
#include <linux/in.h>
#include <cstring>

namespace os
{
	
	template <class _So>
	const auto ip(_So sock, const char *if_name, bool *ok = {})
	{
		ifreq ifr {}; std::strcpy(ifr.ifr_name, if_name);
		const auto res {
			ioctl(sock.descriptor(), SIOCGIFADDR, &ifr) >= 0
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
	const auto netmask(_So sock, const char *if_name, bool *ok = {})
	{
		ifreq ifr; std::strcpy(ifr.ifr_name, if_name);
		const auto res {
			ioctl(sock.descriptor(), SIOCGIFNETMASK, &ifr) >= 0
		};
		const auto s_addr {
			reinterpret_cast<sockaddr_in*>(&ifr.ifr_netmask)
		};
		if (ok) {
			*ok = res;
		}
		return s_addr->sin_addr;
	}

} // os

#endif // __SOCKIOS_H__
