#include <cstddef>
#include <iostream>
#include <string>

#include "connected_socket.h"
#include "socket.h"

namespace {

constexpr std::uint16_t kPort = 9090;

} // namespace

auto main() -> int {
    net::Socket socket(net::Socket::Type::Tcp);
    auto connected_socket = socket.connect("127.0.0.1", kPort);
    if (!connected_socket) {
        std::cerr << "Failed to connect to server." << "\n";
        return 1;
    }

    std::cout << "Sending message..." << "\n";
    std::cout.flush();

    const std::string message = "hello world";
    auto bytes_sent = connected_socket->send(message);
    if (bytes_sent < 0) {
        std::cerr << "Failed to send message." << "\n";
        return 1;
    }

    std::cout << "Sent: " << message << "\n";

    std::string reply;
    constexpr std::size_t reply_capacity = 1024;
    auto bytes_read = connected_socket->recv(reply, reply_capacity);
    if (bytes_read < 0) {
        std::cerr << "Failed to receive reply." << "\n";
        return 1;
    }

    std::cout << "Received: " << reply << "\n";
    return 0;
}
