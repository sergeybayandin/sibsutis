#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <cassert>
#include <utility>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

namespace os
{

	namespace details
	{

		template <class _So, class _Tp>
		bool bind_afpf_packet(const _So &sock, _Tp ifindex) noexcept 
		{
			sockaddr_ll sll {
				.sll_family = static_cast<unsigned short>(sock.domain()),
				.sll_protocol = sock.protocol(),
				.sll_ifindex = ifindex
			};

			return
				::bind(
					sock.descriptor(),
					reinterpret_cast<sockaddr*>(&sll),
					sizeof(sockaddr_ll)
				) >= 0;
		}

		template <auto _Dn, class _So, class ...Args>
		bool bind_impl(const _So &sock, Args &&...args) noexcept
		{
			if constexpr (_Dn == AF_PACKET || _Dn == PF_PACKET) {
				return bind_afpf_packet(sock, std::forward<Args>(args)...);
			}
		
			/*
		
				TODO: domains

			*/
		}

	} // details


	template <auto _Dn, auto _Tp, auto _Pc>
	class unix_socket final
	{
	public:
		unix_socket() noexcept;
		~unix_socket() noexcept;
		
		unix_socket(unix_socket&&) = delete;
		unix_socket(const unix_socket&) = delete;
		unix_socket &operator=(const unix_socket&) = delete;

		bool open() noexcept;
		bool close() noexcept;
		
		template <class ...Args>
		bool bind(Args &&...args) const noexcept {
			return details::bind_impl<_Dn>(*this, std::forward<Args>(args)...);
		}

		ssize_t recv(char *buff, std::size_t buff_size, int flags = {}) const noexcept;

		const int descriptor() const noexcept;
		constexpr auto domain() const noexcept;
		constexpr auto type() const noexcept;
		constexpr auto protocol() const noexcept;

	private:
		int sd_;
	};

	template <auto _Dn, auto _Tp, auto _Pc>
	constexpr auto 
	unix_socket<_Dn, _Tp, _Pc>::domain() const noexcept
	{
		return _Dn;
	}

	template <auto _Dn, auto _Tp, auto _Pc>
	constexpr auto 
	unix_socket<_Dn, _Tp, _Pc>::type() const noexcept
	{
		return _Tp;
	}

	template <auto _Dn, auto _Tp, auto _Pc>
	constexpr auto 
	unix_socket<_Dn, _Tp, _Pc>::protocol() const noexcept
	{
		return _Pc;
	}

	template <auto _Dn, auto _Tp, auto _Pc>
	bool
	unix_socket<_Dn, _Tp, _Pc>::open() noexcept
	{
		return 
			(sd_ = ::socket(
				domain(),
				type(),
				protocol()
			)) >= 0;
	}

	template <auto _Dn, auto _Tp, auto _Pc>
	const int
	unix_socket<_Dn, _Tp, _Pc>::descriptor() const noexcept
	{
		assert(sd_ >= 0);
		return sd_;
	}
	
	template <auto _Dn, auto _Tp, auto _Pc>
	unix_socket<_Dn, _Tp, _Pc>::unix_socket() noexcept
		: sd_ {-1}
	{}
	
	template <auto _Dn, auto _Tp, auto _Pc>
	bool
	unix_socket<_Dn, _Tp, _Pc>::close() noexcept
	{
		return ::close(sd_) >= 0;
	}
	
	template <auto _Dn, auto _Tp, auto _Pc>
	unix_socket<_Dn, _Tp, _Pc>::~unix_socket() noexcept
	{
		close();
	}

	template <auto _Dn, auto _Tp, auto _Pc>
	ssize_t unix_socket<_Dn, _Tp, _Pc>::recv(char *buff, std::size_t buff_size, int flags) const noexcept
	{
		return ::recv(sd_, buff, buff_size, flags);
	}

} // os

#endif // __SOCKET_H__
