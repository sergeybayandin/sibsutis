#include <asio.hpp>

#include <list>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <source_location>

struct connection final
{
    asio::ip::tcp::socket socket;
    std::array<char, 8> buffer;

    explicit connection(auto &&s) noexcept
        : socket {std::move(s)}
    {}
};

std::list<connection> connections;

void receive(connection &conn) noexcept
{
    std::ranges::fill(conn.buffer, 0x00);
    conn.socket.async_receive(
        asio::buffer(conn.buffer.data(), conn.buffer.size()),
        [&conn](asio::error_code ec, std::size_t nread) {
            if (!ec) {
                std::cout << "Адрес клиента: "
                          << conn.socket.local_endpoint() << '\n';
                std::cout << "Длина сообщения: " << nread << '\n';
                std::cout << "Сообщение: " << conn.buffer.data() << '\n';
            }
            receive(conn);
        }
    );
}

void accept_client(asio::ip::tcp::acceptor &acceptor) noexcept
{
    acceptor.async_accept(
        [&acceptor](asio::error_code ec, auto socket) {
            if (!ec) {
                connections.emplace_back(std::move(socket));
                receive(connections.back());
                accept_client(acceptor);
            }
        }
    );
}

int main()
{
    asio::error_code ec;
    asio::io_context context;
   
    asio::ip::tcp::endpoint endpoint {
        asio::ip::tcp::v4(), 0
    };
    asio::ip::tcp::acceptor acceptor {context, endpoint.protocol()};
    
    acceptor.bind(endpoint, ec);

    if (ec) {
        std::cerr << "Failed(" << std::source_location::current().line() << "): " << ec.message() << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "Адрес сервера: " << acceptor.local_endpoint() << "\n\n";

    acceptor.listen(5);

    accept_client(acceptor);

    context.run();

    return EXIT_SUCCESS;
}
