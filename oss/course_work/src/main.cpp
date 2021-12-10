#include "socket.h"
#include "sockios.h"
#include "tr.h"
#include <iostream>
#include <signal.h>
#include <linux/ip.h>
#include <iomanip>
#include <linux/tcp.h>

bool running = true;

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "missing ifname\n";
		return EXIT_FAILURE;
	}

	os::unix_socket<AF_INET, SOCK_DGRAM, 0> dgram_sock;

	if (!dgram_sock.open()) {
		std::cerr << "failed dgram_sock.open()\n";
		return EXIT_FAILURE;
	}

	bool ok;
	auto ip {
		os::get_ip(dgram_sock, argv[1], &ok)
	};

	if (!ok) {
		std::cerr << "failed os::get_ip()\n";
		return EXIT_FAILURE;
	}

	std::cout << "ip address: " << os::inet_ntoa(ip) << '\n';

	auto netmask {
		os::get_netmask(dgram_sock, argv[1], &ok)
	};

	if (!ok) {
		std::cerr << "failed os::get_netmask()\n";
		return EXIT_FAILURE;
	}

	std::cout << "netmask: " << os::inet_ntoa(netmask) << '\n';

	auto mtu {
		os::get_mtu(dgram_sock, argv[1], &ok)
	};

	if (!ok) {
		std::cerr << "failed os::get_mtu()\n";
		return EXIT_FAILURE;
	}

	std::cout << "mtu: " << mtu << '\n';

	auto ifindex {
		os::get_ifindex(dgram_sock, argv[1], &ok)
	};

	if (!ok) {
		std::cerr << "failed os::get_ifindex()\n";
		return EXIT_FAILURE;
	}

	std::cout << "ifindex: " << ifindex << '\n';

	os::unix_socket<
		PF_PACKET,
		SOCK_RAW,
		os::hton<unsigned short>(ETH_P_ALL)
	> pack_sock;

	if (!pack_sock.open()) {
		std::cerr << "failed pack_sock.open()\n";
		return EXIT_FAILURE;
	}

	if (!pack_sock.bind(ifindex)) {
		std::cerr << "failed pack_sock.bind()\n";
		return EXIT_FAILURE;
	}

	struct sigaction ctrlc_act;
	
	ctrlc_act.sa_handler = [](int) {
		running = false;
	};
	sigemptyset(&ctrlc_act.sa_mask);
	sigaction(SIGINT, &ctrlc_act, {});
	
	if (!os::change_flags<true>(dgram_sock, argv[1], IFF_PROMISC)) {
		std::cerr << "failed change_flags()" << '\n';
		return EXIT_FAILURE;
	}
	
	iphdr iph;
	ethhdr ethh;
	char frame[ETH_FRAME_LEN];

	std::cout << "\nsniffing...\n\n";

	for (std::size_t i {1}; running; ) {
		std::memset(frame, 0x00, sizeof(frame));

		auto read {pack_sock.recv(frame, mtu + 18)};
		if (read < 0 || read > ETH_FRAME_LEN) {
			std::cerr << "failed pack_sock.recv()" << '\n';
			continue;
		}
		
		std::memcpy(&ethh, frame, ETH_HLEN);
		std::memcpy(&iph, frame + ETH_HLEN, sizeof(iph));
	
		std::cout << i++ << ')' << '\n';

		std::cout << "mac src: " 
			<< std::hex 
			<< static_cast<unsigned>(ethh.h_source[0]) << ':'
			<< static_cast<unsigned>(ethh.h_source[1]) << ':'
			<< static_cast<unsigned>(ethh.h_source[2]) << ':'
			<< static_cast<unsigned>(ethh.h_source[3]) << ':'
			<< static_cast<unsigned>(ethh.h_source[4]) << ':'
			<< static_cast<unsigned>(ethh.h_source[5]) << '\n';		
		std::cout << "mac dest: " 
			<< static_cast<unsigned>(ethh.h_dest[0]) << ':'
			<< static_cast<unsigned>(ethh.h_dest[1]) << ':'
			<< static_cast<unsigned>(ethh.h_dest[2]) << ':'
			<< static_cast<unsigned>(ethh.h_dest[3]) << ':'
			<< static_cast<unsigned>(ethh.h_dest[4]) << ':'
			<< static_cast<unsigned>(ethh.h_dest[5]) << '\n' << std::dec;

		std::cout << "ip src: " << os::inet_ntoa(iph.saddr) << '\n';
		std::cout << "ip dest: " << os::inet_ntoa(iph.daddr) << "\n";

		if (iph.protocol == IPPROTO_TCP) {
			auto tcph {
				reinterpret_cast<tcphdr*>(frame + ETH_HLEN + iph.ihl * 4)
			};
			auto data {
				reinterpret_cast<const unsigned char*>(tcph) + tcph->doff * 4
			};
			std::cout << "data: " << data << "\n\n";
		} else {
			std::cout << '\n';
		}
	}
	
	if (!os::change_flags<false>(dgram_sock, argv[1], IFF_PROMISC)) {
		std::cerr << "failed change_flags()\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
