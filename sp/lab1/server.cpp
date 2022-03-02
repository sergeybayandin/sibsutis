#include <array>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main()
{
    auto const sd {socket(AF_INET, SOCK_DGRAM, 0)};

    assert(sd >= 0);

    sockaddr_in sin;

    std::fill_n(
        reinterpret_cast<std::byte*>(&sin),
        sizeof(sin),
        std::byte{}
    );

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = 0;

    assert(
        !bind(sd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin))
    );

    socklen_t addrlen {sizeof(sin)};

    assert(
        !getsockname(sd, reinterpret_cast<sockaddr*>(&sin), &addrlen)
    );

    std::cout << "IP адрес: " << inet_ntoa(sin.sin_addr) << '\n'
              << "Порт: " << ntohs(sin.sin_port) << "\n\n";
    
    std::array<char, 8> buf;

    while (true) {
        std::ranges::fill(buf, 0x00);

        auto const read {
            recvfrom(
                sd,
                buf.data() + 1,
                buf.size() - 1,
                0, 
                reinterpret_cast<sockaddr*>(&sin),
                &(addrlen = sizeof(sin))
            )
        };

        assert(read > 0);

        std::cout << "IP адрес клиента: " << inet_ntoa(sin.sin_addr) << '\n'
                  << "Порт клиента: " << ntohs(sin.sin_port) << '\n'
                  << "Длина сообщения: " << read << '\n'
                  << "Сообщение: " << buf.data() + 1 << "\n\n";

        buf[0] = '-';

        assert(
            sendto(
                sd,
                buf.data(),
                read + 1,
                0,
                reinterpret_cast<sockaddr*>(&sin),
                sizeof(sin)
            ) >= 0
        );
    }

    assert(!close(sd));

    return EXIT_SUCCESS;
}
