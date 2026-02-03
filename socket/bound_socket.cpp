#include "bound_socket.h"

#include "connected_socket.h"
#include "socket.h"

namespace net {

BoundSocket::BoundSocket(Socket &&socket, int backlog)
    : socket_(std::move(socket)) {
    socket_.listen(backlog);
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

auto BoundSocket::accept() -> ConnectedSocket {
    return ConnectedSocket(socket_.accept());
}

} // namespace net
