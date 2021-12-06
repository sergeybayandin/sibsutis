#ifndef __DOMAIN_H__
#define __DOMAIN_H__

#include <sys/types.h>

namespace os
{

	namespace details
	{

		template <auto _Dn>
		struct unix_domain
		{
			static constexpr auto value {_Dn};
		};

	} // details

	struct pf_local
		: details::unix_domain<PF_LOCAL>
	{};

	struct pf_inet
		: details::unix_domain<PF_INET>
	{};

	struct pf_inet6
		: details::unix_domain<PF_INET6>
	{};

	struct pf_ipx
		: details::unix_domain<PF_IPX>
	{};

	struct pf_netlink
		: details::unix_domain<PF_NETLINK>
	{};

	struct pf_x25
		: details::unix_domain<PF_X25>
	{};

	struct pf_ax25
		: details::unix_domain<PF_AX25>
	{};

	struct pf_atmpvc
		: details::unix_domain<PF_ATMPVC>
	{};

	struct pf_appletalk
		: details::unix_domain<PF_APPLETALK>
	{};

	struct pf_packet
		: details::unix_domain<PF_PACKET>
	{};

} // os

#endif // __DOMAIN_H__
