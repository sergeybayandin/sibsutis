#include <asio.hpp>

#include <thread>
#include <iostream>
#include <source_location>

int main(int argc, char *argv[])
{
    asio::error_code ec;
    asio::io_context context;
    asio::ip::tcp::socket socket {context};

    socket.connect(
        asio::ip::tcp::endpoint(asio::ip::make_address(argv[1]), std::atoi(argv[2])),
        ec
    );

    if (ec) {
        std::cerr << "Failed(" << std::source_location::current().line() << "): " << ec.message() << '\n';
        return EXIT_FAILURE;
    }

    int ntimes {std::atoi(argv[4])};
    int const i {std::atoi(argv[3])};
    auto const length {std::strlen(argv[3])};

    while (ntimes--) {
        auto const nread {socket.send(asio::buffer(argv[3], length))};

        if (!nread) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds{i});
    }

    return EXIT_SUCCESS;
}
