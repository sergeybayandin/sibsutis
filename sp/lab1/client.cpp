#include <thread>
#include <netdb.h>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <charconv>
#include <algorithm>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    assert(argc == 5);

    auto const sd {socket(AF_INET, SOCK_DGRAM, 0)};

    assert(sd > 0);

    sockaddr_in sin;

    std::fill_n(
        reinterpret_cast<std::byte*>(&sin),
        sizeof(sin),
        std::byte{0x00}
    );

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = 0;

    assert(
       !bind(sd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin))
    );

    std::fill_n(
        reinterpret_cast<std::byte*>(&sin),
        sizeof(sin),
        std::byte{0x00}
    );

    auto const hs {gethostbyname(argv[1])};

    sin.sin_family = AF_INET;
    std::copy_n(
        reinterpret_cast<std::byte*>(hs->h_addr),
        hs->h_length,
        reinterpret_cast<std::byte*>(&sin.sin_addr)
    );
    sin.sin_port = htons(std::atoi(argv[2]));

    socklen_t addrlen;
    auto const slen {std::strlen(argv[3])};
    std::array<char, 8> buf;
    auto i {std::atoi(argv[3])};

    for (auto ntimes {std::atoi(argv[4])}; ntimes; --ntimes) {
        assert(
            sendto(
                sd,
                argv[3],
                slen,
                0,
                reinterpret_cast<sockaddr*>(&sin),
                sizeof(sin)
            ) >= 0
        );

        std::ranges::fill(buf, 0x00);

        assert(
            recvfrom(
                sd,
                buf.data(),
                buf.size(),
                0,
                reinterpret_cast<sockaddr*>(&sin),
                &(addrlen = sizeof(sin))
            )
        );

        std::cout << "Ответ сервера: " << buf.data() << '\n';

        std::this_thread::sleep_for(
            std::chrono::seconds{i}
        );
    }
 
    assert(!close(sd));
 
    return EXIT_SUCCESS;
}
