#include "connected_socket.h"

#include "socket.h"

#include <sys/socket.h>
#include <vector>

namespace net {

ConnectedSocket::ConnectedSocket(Socket &&socket)
    : socket_(std::move(socket)) {}

ConnectedSocket::ConnectedSocket(int fd, Socket::Type type) noexcept {
    socket_ = Socket(fd, type);
}

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
                           std::size_t len) const -> std::ptrdiff_t {
    if (!is_valid()) {
        return -1;
    }
    return ::send(fd(), data, len, MSG_NOSIGNAL);
}

auto ConnectedSocket::send(const std::string &data) const -> std::ptrdiff_t {
    return send(data.data(), data.size());
}

auto ConnectedSocket::recv(void *buffer,
                           std::size_t len) const -> std::ptrdiff_t {
    if (!is_valid()) {
        return -1;
    }
    return ::recv(fd(), buffer, len, 0);
}

auto ConnectedSocket::recv(std::string &out,
                           std::size_t max_len) const -> std::ptrdiff_t {
    out.clear();
    if (max_len == 0) {
        return 0;
    }

    std::vector<char> buffer(max_len);
    auto bytes = recv(buffer.data(), buffer.size());
    if (bytes > 0) {
        out.assign(buffer.data(), static_cast<std::size_t>(bytes));
    }
    return bytes;
}

} // namespace net
