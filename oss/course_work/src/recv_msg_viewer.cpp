#include "recv_msg_viewer.h"
#include <cassert>

namespace os
{

	namespace details
	{

		const std::uint8_t*
		eth_hdr::get_destination_address() const noexcept
		{
			return h_dest;
		}

		const std::uint8_t*
		eth_hdr::get_source_address() const noexcept
		{
			return h_source;
		}

		std::uint16_t
		eth_hdr::get_proto() const noexcept
		{
			return h_proto;
		}

		std::uint32_t
		ip_hdr::get_source_address() const noexcept
		{
			return saddr;
		}

		std::uint32_t
		ip_hdr::get_destination_address() const noexcept
		{
			return daddr;
		}

		std::uint8_t
		ip_hdr::get_protocol() const noexcept
		{
			return protocol;
		}

		std::uint8_t
		ip_hdr::get_version() const noexcept
		{
			return version;
		}

		std::uint8_t
		ip_hdr::get_ihl() const noexcept
		{
			return ihl;
		}

		std::uint16_t
		tcp_hdr::get_doff() const noexcept
		{
			return doff;
		}

		const std::uint8_t*
		tcp_hdr::get_data() const noexcept
		{
			return
				reinterpret_cast<const uint8_t*>(this) + 4 * get_doff();
		}

		std::uint16_t
		tcp_hdr::get_source() const noexcept
		{
			return source;
		}

		std::uint16_t
		tcp_hdr::get_dest() const noexcept
		{
			return dest;
		}

	} // details

	recv_msg_viewer::recv_msg_viewer(const char *buff) noexcept
		: buff_ {buff}
	{}

	const details::eth_hdr*
	recv_msg_viewer::eth_hdr() const noexcept
	{
		assert(buff_);
		return
			reinterpret_cast<const details::eth_hdr*>(buff_);
	}

	const details::ip_hdr*
	recv_msg_viewer::ip_hdr() const noexcept
	{
		assert(buff_);
		return
			reinterpret_cast<const details::ip_hdr*>(buff_ + ETH_HLEN);
	}

	const details::tcp_hdr*
	recv_msg_viewer::tcp_hdr() const noexcept
	{
		assert(buff_);
		return
			reinterpret_cast<const details::tcp_hdr*>(buff_ + ETH_HLEN + 4 * ip_hdr()->get_ihl());
	}

} // os
