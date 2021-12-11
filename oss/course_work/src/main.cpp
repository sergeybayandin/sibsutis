#include "socket.h"
#include "sockios.h"
#include "tr.h"
#include "recv_msg_viewer.h"
#include <iostream>
#include <signal.h>
#include <iomanip>

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
	//ethhdr ethh;
	char frame[ETH_FRAME_LEN];

	std::cout << "\nsniffing...\n\n";

	for (std::size_t i {1}; running; ) {
		std::memset(frame, 0x00, sizeof(frame));

		auto read {pack_sock.recv(frame, mtu + 18)};
		if (read < 0 || read > ETH_FRAME_LEN) {
			std::cerr << "failed pack_sock.recv()" << '\n';
			continue;
		}
		
		std::cout << i++ << ')' << '\n';
		
		os::recv_msg_viewer viewer {frame};

		auto ip_hdr {
			viewer.ip_hdr()
		};

		auto eth_hdr {
			viewer.eth_hdr()
		};
	
		std::cout << "mac:\t" << std::setw(25) << std::left << os::eth_ntoa(eth_hdr->get_source_address());
		std::cout << "-------->";
		std::cout << std::setw(25) << std::right << os::eth_ntoa(eth_hdr->get_destination_address()) << '\n';

		std::cout << "ip:\t" << std::setw(25) << std::left << os::inet_ntoa(ip_hdr->get_source_address());
		std::cout << "-------->";
		std::cout << std::setw(25) << std::right << os::inet_ntoa(ip_hdr->get_destination_address()) << "\n";	
		
		if (ip_hdr->get_protocol() == IPPROTO_TCP) {
			auto tcp_hdr {
				viewer.tcp_hdr()
			};
			std::cout << "data:\t" << viewer.tcp_hdr()->get_data() << "\n\n";
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
