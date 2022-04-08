#include <wait.h>
#include <cassert>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    auto const sd {socket(AF_INET, SOCK_STREAM, 0)};

    assert(sd >= 0);

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

    socklen_t addrlen {sizeof(sin)};

    assert(
        !getsockname(sd, reinterpret_cast<sockaddr*>(&sin), &addrlen)
    );

    std::cout << "IP адрес: " << inet_ntoa(sin.sin_addr) << '\n'
              << "Порт: " << ntohs(sin.sin_port) << "\n\n";

    int cd;

    auto kill_zombies {
        [](int) -> void {
            pid_t pid;
            do {
                pid = wait3(nullptr, WNOHANG, nullptr);
            } while (pid != -1 && pid);
        }
    };
    signal(SIGCHLD, kill_zombies);
        
    assert(!listen(sd, 5));

    while (true) {
        cd = accept(sd, reinterpret_cast<sockaddr*>(&sin), &addrlen);
  
        assert(cd >= 0);

        auto const pid {fork()};

        if (pid) {
            assert(pid > 0);
        } else {
            break;
        }
    }
 
    std::array<char, 8> buf;

    while (true) {
        std::ranges::fill(buf, 0x00);
 
        auto const read {
            recv(
                cd,
                buf.data() + 1,
                buf.size() - 1,
                0
            )
        };

        if (read <= 0) {
            assert(!close(cd));
            return EXIT_SUCCESS;
        }

        std::cout << "ID процесса: " << getpid() << '\n'
                  << "IP адрес клиента: " << inet_ntoa(sin.sin_addr) << '\n'
                  << "Порт клиента: " << ntohs(sin.sin_port) << '\n'
                  << "Длина сообщения: " << read << '\n'
                  << "Сообщение: " << buf.data() + 1 << "\n\n";
    }

    return EXIT_SUCCESS;
}
