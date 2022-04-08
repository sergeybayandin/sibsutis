#include <list>
#include <cassert>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <algorithm>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

struct worker_arg final
{
    int cd;
    sockaddr_in sin;
    typename std::list<worker_arg>::iterator it;
};

std::list<worker_arg> worker_args;

void *worker(void *pwa)
{
    std::array<char, 8> buf;
    auto const tid {pthread_self()};
    auto const wa {static_cast<worker_arg*>(pwa)};
    pthread_mutex_t mutex(PTHREAD_MUTEX_INITIALIZER);

    while (true) {
        std::ranges::fill(buf, 0x00);

        auto const read {
            recv(wa->cd, buf.data(), buf.size(), 0)
        };

        if (read <= 0) {
            break;
        }

        pthread_mutex_lock(&mutex);

        std::cout << "ID потока: " << tid << '\n'
                  << "IP адрес клиента: " << inet_ntoa(wa->sin.sin_addr) << '\n'
                  << "Порт клиента: " << ntohs(wa->sin.sin_port) << '\n'
                  << "Длина сообщения: " << read << '\n'
                  << "Сообщение: " << buf.data() << "\n\n";
        
        std::ofstream file {".log", std::ios_base::app};

        file << "ID потока: " << tid << '\n'
             << "IP адрес клиента: " << inet_ntoa(wa->sin.sin_addr) << '\n'
             << "Порт клиента: " << ntohs(wa->sin.sin_port) << '\n'
             << "Длина сообщения: " << read << '\n'
             << "Сообщение: " << buf.data() << "\n\n";
        
        file.close();

        pthread_mutex_unlock(&mutex);
    }
    
    assert(!close(wa->cd));
    worker_args.erase(wa->it);

    pthread_exit(nullptr);
}

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

    std::cout << "IP адрес : " << inet_ntoa(sin.sin_addr) << '\n'
              << "Порт: " << ntohs(sin.sin_port) << "\n\n";

    worker_arg wa;
 
    assert(!listen(sd, 5));

    while (true) {
        std::fill_n(
            reinterpret_cast<std::byte*>(&wa),
            sizeof(wa),
            std::byte{0x00}
        );
        addrlen = sizeof(wa);

        wa.cd = accept(sd, reinterpret_cast<sockaddr*>(&wa.sin), &addrlen);

        assert(wa.cd >= 0);

        worker_args.push_back(wa);
        worker_args.back().it = std::prev(worker_args.end());

        pthread_t tid;

        assert(
            !pthread_create(
                &tid,
                nullptr,
                worker,
                &worker_args.back()
            )
        );
    }

    return EXIT_SUCCESS;
}
