#include "socket.h"

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <vector>

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

Socket::Socket(int fd, Type type) : sock_fd_(fd), type_(type) {}

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

auto Socket::bind(std::uint16_t port, const std::string &address) -> bool {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = to_sock_type(type_);
    hints.ai_protocol = to_protocol(type_);
    hints.ai_flags = AI_PASSIVE;

    addrinfo *result = nullptr;
    std::string port_str = port_to_string(port);
    const char *addr = address.empty() ? nullptr : address.c_str();

    if (::getaddrinfo(addr, port_str.c_str(), &hints, &result) != 0) {
        return false;
    }

    bool bound = false;
    for (addrinfo *cur = result; cur != nullptr; cur = cur->ai_next) {
        int candidate_fd = sock_fd_;
        if (candidate_fd < 0) {
            candidate_fd =
                ::socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
            if (candidate_fd < 0) {
                continue;
            }
        }

        int reuse = 1;
        ::setsockopt(candidate_fd, SOL_SOCKET, SO_REUSEADDR, &reuse,
                     sizeof(reuse));

        if (::bind(candidate_fd, cur->ai_addr, cur->ai_addrlen) == 0) {
            if (sock_fd_ != candidate_fd) {
                close();
                sock_fd_ = candidate_fd;
            }
            bound = true;
            break;
        }

        if (candidate_fd != sock_fd_) {
            ::close(candidate_fd);
        } else {
            close();
        }
    }

    ::freeaddrinfo(result);
    return bound;
}

auto Socket::listen(int backlog) -> bool {
    if (type_ != Type::Tcp || sock_fd_ < 0) {
        return false;
    }
    return ::listen(sock_fd_, backlog) == 0;
}

auto Socket::accept() -> Socket {
    if (type_ != Type::Tcp || sock_fd_ < 0) {
        return Socket();
    }

    int client_fd = ::accept(sock_fd_, nullptr, nullptr);
    if (client_fd < 0) {
        return Socket();
    }

    return Socket(client_fd, type_);
}

auto Socket::recv(void *buffer, std::size_t len) -> std::ptrdiff_t {
    if (sock_fd_ < 0) {
        return -1;
    }
    return ::recv(sock_fd_, buffer, len, 0);
}

auto Socket::recv(std::string &out, std::size_t max_len) -> std::ptrdiff_t {
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
