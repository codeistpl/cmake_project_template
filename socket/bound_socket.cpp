#include "bound_socket.h"

#include "connected_socket.h"
#include "socket.h"

#include <sys/socket.h>
#include <unistd.h>

namespace net {

BoundSocket::BoundSocket(Socket &&socket, int backlog)
    : socket_(std::move(socket)) {
    listen(backlog);
}

BoundSocket::BoundSocket(BoundSocket &&other) noexcept
    : socket_(std::move(other.socket_)) {}

auto BoundSocket::operator=(BoundSocket &&other) noexcept -> BoundSocket & {
    if (this != &other) {
        socket_ = std::move(other.socket_);
    }
    return *this;
}

auto BoundSocket::is_valid() const -> bool { return socket_.is_valid(); }

auto BoundSocket::fd() const -> int { return socket_.fd(); }

auto BoundSocket::close() -> void { socket_.close(); }

auto BoundSocket::listen(int backlog) -> bool {
    if (socket_.type_ != Socket::Type::Tcp || socket_.sock_fd_ < 0) {
        return false;
    }
    return ::listen(socket_.sock_fd_, backlog) == 0;
}

auto BoundSocket::accept() -> ConnectedSocket {
    if (socket_.type_ != Socket::Type::Tcp || socket_.sock_fd_ < 0) {
        return ConnectedSocket(Socket());
    }

    int client_fd = ::accept(socket_.sock_fd_, nullptr, nullptr);
    if (client_fd < 0) {
        return ConnectedSocket(Socket());
    }

    return ConnectedSocket(client_fd, socket_.type_);
}

} // namespace net
