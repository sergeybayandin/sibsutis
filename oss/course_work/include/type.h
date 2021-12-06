#ifndef __TYPE_H__
#define __TYPE_H__

#include <sys/socket.h>

namespace os
{

	namespace details
	{

		template <auto _Tp>
		struct unix_type
		{
			static constexpr auto value {_Tp};
		};

	} // details

	struct sock_stream
		: details::unix_type<SOCK_STREAM>
	{};

	struct sock_dgram
		: details::unix_type<SOCK_DGRAM>
	{};

	struct sock_seqpacket
		: details::unix_type<SOCK_SEQPACKET>
	{};

	struct sock_raw
		: details::unix_type<SOCK_RAW>
	{};

	struct sock_rdm
		: details::unix_type<SOCK_RDM>
	{};

	struct sock_packet
		: details::unix_type<SOCK_PACKET>
	{};

} // os

#endif // __TYPE_H__
