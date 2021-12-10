#ifndef __RECV_MSG_VIEWER_H__
#define __RECV_MSG_VIEWER_H__

#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <cstdint>

namespace os
{
	
	namespace details
	{

		class eth_hdr : ethhdr
		{
		public:
			const std::uint8_t*
			get_destination_address() const noexcept;

			const std::uint8_t*
			get_source_address() const noexcept;

			std::uint16_t 
			get_proto() const noexcept;
		};

		class ip_hdr : iphdr
		{
		public:
			std::uint32_t
			get_source_address() const noexcept;

			std::uint32_t
			get_destination_address() const noexcept;

			std::uint8_t
			get_protocol() const noexcept;

			std::uint8_t
			get_version() const noexcept;
		
			std::uint8_t
			get_ihl() const noexcept;
		};

		class tcp_hdr : tcphdr
		{
		public:
			std::uint16_t
			get_doff() const noexcept;

			const uint8_t*
			get_data() const noexcept;
			
			std::uint16_t
			get_source() const noexcept;
		
			std::uint16_t
			get_dest() const noexcept;
		};

	} // details

	class recv_msg_viewer final
	{
	public:
		recv_msg_viewer(const char *buff = {}) noexcept;

		const details::eth_hdr *eth_hdr() const noexcept;
		const details::ip_hdr *ip_hdr() const noexcept;
		const details::tcp_hdr *tcp_hdr() const noexcept;

	private:
		const char *buff_;
	};

} // os

#endif // __RECV_MSG_VIEWER_H__
