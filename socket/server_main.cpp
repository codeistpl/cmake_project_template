// Simple TCP server for local testing
#include <iostream>
#include <string>

#include "socket.h"

namespace {

constexpr std::uint16_t kPort = 9090;
constexpr std::size_t kBufferSize = 1024;

} // namespace

auto main() -> int {
    net::Socket server(net::Socket::Type::Tcp);
    if (!server.bind(kPort, "127.0.0.1")) {
        std::cerr << "Failed to bind server socket." << std::endl;
        return 1;
    }

    if (!server.listen(1)) {
        std::cerr << "Failed to listen on server socket." << std::endl;
        return 1;
    }

    std::cout << "Server listening on 127.0.0.1:" << kPort << std::endl;

    net::Socket client = server.accept();
    if (!client.is_valid()) {
        std::cerr << "Failed to accept client connection." << std::endl;
        return 1;
    }

    std::string message;
    auto bytes_read = client.recv(message, kBufferSize);
    if (bytes_read < 0) {
        std::cerr << "Failed to read from client." << std::endl;
        return 1;
    }

    std::cout << "Received: " << message << std::endl;

    return 0;
}
