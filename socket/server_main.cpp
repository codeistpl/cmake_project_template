// Simple TCP server for local testing
#include <iostream>
#include <string>

#include "bound_socket.h"
#include "connected_socket.h"
#include "socket.h"

namespace {

constexpr std::uint16_t kPort = 9090;
constexpr std::size_t kBufferSize = 1024;

} // namespace

auto main() -> int {
    net::Socket server(net::Socket::Type::Tcp);
    if (!server.bind(kPort, "127.0.0.1")) {
        std::cerr << "Failed to bind server socket." << '\n';
        return 1;
    }

    net::BoundSocket bound_server(std::move(server), 1);
    std::cout << "Server listening on 127.0.0.1:" << kPort << '\n';
    std::cout.flush();

    std::cout << "Waiting for client..." << '\n';
    std::cout.flush();
    net::ConnectedSocket client = bound_server.accept();
    std::cout << "Client accepted" << '\n';
    std::cout.flush();
    if (!client.is_valid()) {
        std::cerr << "Failed to accept client connection." << '\n';
        return 1;
    }

    std::string message;
    auto bytes_read = client.recv(message, kBufferSize);
    if (bytes_read < 0) {
        std::cerr << "Failed to read from client." << '\n';
        return 1;
    }

    std::cout << "Received: " << message << '\n';

    const std::string reply = "Hi There";
    auto bytes_sent = client.send(reply);
    if (bytes_sent < 0) {
        std::cerr << "Failed to send reply." << '\n';
        return 1;
    }

    std::cout << "Sent: " << reply << '\n';

    return 0;
}
