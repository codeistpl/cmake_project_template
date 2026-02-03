#include <iostream>
#include <string>

#include "socket.h"

namespace {

constexpr std::uint16_t kPort = 9090;

} // namespace

auto main() -> int {
    net::Socket socket(net::Socket::Type::Tcp);
    if (!socket.connect("127.0.0.1", kPort)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    const std::string message = "hello world";
    auto bytes_sent = socket.send(message);
    if (bytes_sent < 0) {
        std::cerr << "Failed to send message." << std::endl;
        return 1;
    }

    std::cout << "Sent: " << message << std::endl;
    return 0;
}
