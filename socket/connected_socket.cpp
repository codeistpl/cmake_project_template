#include "connected_socket.h"

#include "socket.h"

namespace net {

ConnectedSocket::ConnectedSocket(Socket &&socket)
    : socket_(std::move(socket)) {}

ConnectedSocket::ConnectedSocket(ConnectedSocket &&other) noexcept
    : socket_(std::move(other.socket_)) {}

auto ConnectedSocket::operator=(ConnectedSocket &&other) noexcept
    -> ConnectedSocket & {
    if (this != &other) {
        socket_ = std::move(other.socket_);
    }
    return *this;
}

auto ConnectedSocket::is_valid() const -> bool { return socket_.is_valid(); }

auto ConnectedSocket::fd() const -> int { return socket_.fd(); }

auto ConnectedSocket::close() -> void { socket_.close(); }

auto ConnectedSocket::send(const void *data,
                           std::size_t len) -> std::ptrdiff_t {
    return socket_.send(data, len);
}

auto ConnectedSocket::send(const std::string &data) -> std::ptrdiff_t {
    return socket_.send(data);
}

auto ConnectedSocket::recv(void *buffer, std::size_t len) -> std::ptrdiff_t {
    return socket_.recv(buffer, len);
}

auto ConnectedSocket::recv(std::string &out,
                           std::size_t max_len) -> std::ptrdiff_t {
    return socket_.recv(out, max_len);
}

} // namespace net
