#ifndef __ETHERNET_PROTOCOL_H__
#define __ETHERNET_PROTOCOL_H__

#include <linux/if_ether.h>

namespace os
{

	namespace details
	{

		template <auto _Pc>
		struct unix_ethernet_protocol
		{
			static constexpr auto value {_Pc};
		};
	
	} // details

	struct eth_p_zero
		: details::unix_ethernet_protocol<0>
	{};

	struct eth_p_loop
		: details::unix_ethernet_protocol<ETH_P_LOOP>
	{};

	struct eth_p_pup
		: details::unix_ethernet_protocol<ETH_P_PUP>
	{};
	
	struct eth_p_pupat
		: details::unix_ethernet_protocol<ETH_P_PUPAT>
	{};	

	struct eth_p_ip
		: details::unix_ethernet_protocol<ETH_P_IP>
	{};

	struct eth_p_x25
		: details::unix_ethernet_protocol<ETH_P_X25>
	{};	

	struct eth_p_arp
		: details::unix_ethernet_protocol<ETH_P_ARP>
	{};	

	struct eth_p_bpq
		: details::unix_ethernet_protocol<ETH_P_BPQ>
	{};	

	struct eth_p_ieeepup
		: details::unix_ethernet_protocol<ETH_P_IEEEPUP>
	{};	

	struct eth_p_ieeepupat
		: details::unix_ethernet_protocol<ETH_P_IEEEPUPAT>
	{};	

	struct eth_p_dec
		: details::unix_ethernet_protocol<ETH_P_DEC>
	{};	

	struct eth_p_dna_dl
		: details::unix_ethernet_protocol<ETH_P_DNA_DL>
	{};	

	struct eth_p_dna_rc
		: details::unix_ethernet_protocol<ETH_P_DNA_RC>
	{};	
	
	struct eth_p_dna_rt
		: details::unix_ethernet_protocol<ETH_P_DNA_RT>
	{};	
	
	struct eth_p_lat
		: details::unix_ethernet_protocol<ETH_P_LAT>
	{};	
	
	struct eth_p_diag
		: details::unix_ethernet_protocol<ETH_P_DIAG>
	{};

	struct eth_p_cust
		: details::unix_ethernet_protocol<ETH_P_CUST>
	{};	

	struct eth_p_sca
		: details::unix_ethernet_protocol<ETH_P_SCA>
	{};	

	struct eth_p_teb
		: details::unix_ethernet_protocol<ETH_P_TEB>
	{};	

	struct eth_p_rarp
		: details::unix_ethernet_protocol<ETH_P_RARP>
	{};

	struct eth_p_atalk
		: details::unix_ethernet_protocol<ETH_P_ATALK>
	{};

	struct eth_p_aarp
		: details::unix_ethernet_protocol<ETH_P_AARP>
	{};	

	struct eth_p_8021q
		: details::unix_ethernet_protocol<ETH_P_8021Q>
	{};	

	struct eth_p_ipx
		: details::unix_ethernet_protocol<ETH_P_IPX>
	{};	

	struct eth_p_ipv6
		: details::unix_ethernet_protocol<ETH_P_IPV6>
	{};	

	struct eth_p_pause
		: details::unix_ethernet_protocol<ETH_P_PAUSE>
	{};

	struct eth_p_slow
		: details::unix_ethernet_protocol<ETH_P_SLOW>
	{};

	struct eth_p_wccp
		: details::unix_ethernet_protocol<ETH_P_WCCP>
	{};	

	struct eth_p_ppp_disc
		: details::unix_ethernet_protocol<ETH_P_PPP_DISC>
	{};	

	struct eth_p_ppp_ses
		: details::unix_ethernet_protocol<ETH_P_PPP_SES>
	{};	
	
	struct eth_p_mpls_uc
		: details::unix_ethernet_protocol<ETH_P_MPLS_UC>
	{};	

	struct eth_p_mpls_mc
		: details::unix_ethernet_protocol<ETH_P_MPLS_MC>
	{};	

	struct eth_p_atmmpoa
		: details::unix_ethernet_protocol<ETH_P_ATMMPOA>
	{};	

	struct eth_p_atmfate
		: details::unix_ethernet_protocol<ETH_P_ATMFATE>
	{};

	struct eth_p_pae
		: details::unix_ethernet_protocol<ETH_P_PAE>
	{};

	struct eth_p_aoe
		: details::unix_ethernet_protocol<ETH_P_AOE>
	{};

	struct eth_p_tipc
		: details::unix_ethernet_protocol<ETH_P_TIPC>
	{};

	struct eth_p_1588
		: details::unix_ethernet_protocol<ETH_P_1588>
	{};	

	struct eth_p_fcoe
		: details::unix_ethernet_protocol<ETH_P_FCOE>
	{};	

	struct eth_p_fip
		: details::unix_ethernet_protocol<ETH_P_FIP>
	{};	

	struct eth_p_edsa
		: details::unix_ethernet_protocol<ETH_P_EDSA>
	{};

	struct eth_p_802_3
		: details::unix_ethernet_protocol<ETH_P_802_3>
	{};

	struct eth_p_ax25
		: details::unix_ethernet_protocol<ETH_P_AX25>
	{};

	struct eth_p_all
		: details::unix_ethernet_protocol<ETH_P_ALL>
	{};	

	struct eth_p_802_2
		: details::unix_ethernet_protocol<ETH_P_802_2>
	{};	

	struct eth_p_snap
		: details::unix_ethernet_protocol<ETH_P_SNAP>
	{};	
	
	struct eth_p_ddcmp
		: details::unix_ethernet_protocol<ETH_P_DDCMP>
	{};	

	struct eth_p_wan_ppp
		: details::unix_ethernet_protocol<ETH_P_WAN_PPP>
	{};	

	struct eth_p_ppp_mp
		: details::unix_ethernet_protocol<ETH_P_PPP_MP>
	{};	

	struct eth_p_localtalk
		: details::unix_ethernet_protocol<ETH_P_LOCALTALK>
	{};

	struct eth_p_can
		: details::unix_ethernet_protocol<ETH_P_CAN>
	{};

	struct eth_p_ppptalk
		: details::unix_ethernet_protocol<ETH_P_PPPTALK>
	{};

	struct eth_p_tr_802_2
		: details::unix_ethernet_protocol<ETH_P_TR_802_2>
	{};

	struct eth_p_mobitex
		: details::unix_ethernet_protocol<ETH_P_MOBITEX>
	{};	

	struct eth_p_control
		: details::unix_ethernet_protocol<ETH_P_CONTROL>
	{};	

	struct eth_p_irda
		: details::unix_ethernet_protocol<ETH_P_IRDA>
	{};	

	struct eth_p_econet
		: details::unix_ethernet_protocol<ETH_P_ECONET>
	{};
		
	struct eth_p_hdlc
		: details::unix_ethernet_protocol<ETH_P_HDLC>
	{};	

	struct eth_p_arcnet
		: details::unix_ethernet_protocol<ETH_P_ARCNET>
	{};	

	struct eth_p_dsa
		: details::unix_ethernet_protocol<ETH_P_DSA>
	{};

	struct eth_p_trailer
		: details::unix_ethernet_protocol<ETH_P_TRAILER>
	{};

	struct eth_p_phonet
		: details::unix_ethernet_protocol<ETH_P_PHONET>
	{};

	struct eth_p_ieee802154
		: details::unix_ethernet_protocol<ETH_P_IEEE802154>
	{};

} // os

#endif // __ETHERNET_PROTOCOL_H__
