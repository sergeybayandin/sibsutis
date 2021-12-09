#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <cassert>
#include <unistd.h>
#include <linux/socket.h>

namespace os
{

	template <auto _Dn, auto _Tp, auto _Pc>
	class unix_socket
	{
	public:
		unix_socket() noexcept;
		~unix_socket() noexcept;

		bool open() noexcept;
		bool close() noexcept;

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

} // os

#endif // __SOCKET_H__
