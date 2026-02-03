// Simple TCP server for local testing
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

namespace {

constexpr std::uint16_t kPort = 9090;
constexpr int kBacklog = 1;
constexpr std::size_t kBufferSize = 1024;

} // namespace

auto main() -> int {
    int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Failed to create server socket." << std::endl;
        return 1;
    }

    int reuse = 1;
    if (::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse,
                     sizeof(reuse)) != 0) {
        std::cerr << "Failed to set SO_REUSEADDR." << std::endl;
        ::close(server_fd);
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(kPort);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (::bind(server_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) !=
        0) {
        std::cerr << "Failed to bind server socket." << std::endl;
        ::close(server_fd);
        return 1;
    }

    if (::listen(server_fd, kBacklog) != 0) {
        std::cerr << "Failed to listen on server socket." << std::endl;
        ::close(server_fd);
        return 1;
    }

    std::cout << "Server listening on 127.0.0.1:" << kPort << std::endl;

    int client_fd = ::accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
        std::cerr << "Failed to accept client connection." << std::endl;
        ::close(server_fd);
        return 1;
    }

    std::array<char, kBufferSize> buffer{};
    auto bytes_read = ::recv(client_fd, buffer.data(), buffer.size() - 1, 0);
    if (bytes_read < 0) {
        std::cerr << "Failed to read from client." << std::endl;
        ::close(client_fd);
        ::close(server_fd);
        return 1;
    }

    std::string message(buffer.data(), static_cast<std::size_t>(bytes_read));
    std::cout << "Received: " << message << std::endl;

    ::close(client_fd);
    ::close(server_fd);
    return 0;
}
