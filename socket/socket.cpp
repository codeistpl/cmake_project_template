#include "socket.h"

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

namespace net {

namespace {

auto to_sock_type(Socket::Type type) -> int {
    if (type == Socket::Type::Tcp) {
        return SOCK_STREAM;
    }
    return SOCK_DGRAM;
}

auto to_protocol(Socket::Type type) -> int {
    if (type == Socket::Type::Tcp) {
        return IPPROTO_TCP;
    }
    return IPPROTO_UDP;
}

auto port_to_string(std::uint16_t port) -> std::string {
    return std::to_string(port);
}

} // namespace

Socket::Socket() : sock_fd_(-1), type_(Type::Tcp) {}

Socket::Socket(Type type) : sock_fd_(-1), type_(type) {}

Socket::Socket(Socket &&other) noexcept
    : sock_fd_(other.sock_fd_), type_(other.type_) {
    other.sock_fd_ = -1;
}

auto Socket::operator=(Socket &&other) noexcept -> Socket & {
    if (this != &other) {
        close();
        sock_fd_ = other.sock_fd_;
        type_ = other.type_;
        other.sock_fd_ = -1;
    }
    return *this;
}

Socket::~Socket() { close(); }

auto Socket::create(Type type) -> Socket { return Socket(type); }

auto Socket::is_valid() const -> bool { return sock_fd_ >= 0; }

auto Socket::fd() const -> int { return sock_fd_; }

auto Socket::close() -> void {
    if (sock_fd_ >= 0) {
        ::close(sock_fd_);
        sock_fd_ = -1;
    }
}

auto Socket::connect(const std::string &host, std::uint16_t port) -> bool {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = to_sock_type(type_);
    hints.ai_protocol = to_protocol(type_);

    addrinfo *result = nullptr;
    std::string port_str = port_to_string(port);
    if (::getaddrinfo(host.c_str(), port_str.c_str(), &hints, &result) != 0) {
        return false;
    }

    bool connected = false;
    for (addrinfo *cur = result; cur != nullptr; cur = cur->ai_next) {
        int candidate_fd = sock_fd_;
        if (candidate_fd < 0) {
            candidate_fd =
                ::socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
            if (candidate_fd < 0) {
                continue;
            }
        }

        if (::connect(candidate_fd, cur->ai_addr, cur->ai_addrlen) == 0) {
            if (sock_fd_ != candidate_fd) {
                close();
                sock_fd_ = candidate_fd;
            }
            connected = true;
            break;
        }

        if (candidate_fd != sock_fd_) {
            ::close(candidate_fd);
        } else {
            close();
        }
    }

    ::freeaddrinfo(result);
    return connected;
}

auto Socket::send(const void *data, std::size_t len) -> std::ptrdiff_t {
    if (sock_fd_ < 0) {
        return -1;
    }
    return ::send(sock_fd_, data, len, MSG_NOSIGNAL);
}

auto Socket::send(const std::string &data) -> std::ptrdiff_t {
    return send(data.data(), data.size());
}

} // namespace net
