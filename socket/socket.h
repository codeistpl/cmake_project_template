#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace net {

class Socket {
  public:
    enum class Type { Tcp, Udp };

    Socket();
    explicit Socket(Type type);

    Socket(const Socket &) = delete;
    auto operator=(const Socket &) -> Socket & = delete;

    Socket(Socket &&other) noexcept;
    auto operator=(Socket &&other) noexcept -> Socket &;

    ~Socket();

    static auto create(Type type) -> Socket;

    [[nodiscard]] auto is_valid() const -> bool;
    [[nodiscard]] auto fd() const -> int;
    auto close() -> void;

    // Client-side methods
    auto connect(const std::string &host, std::uint16_t port) -> bool;
    auto send(const void *data, std::size_t len) -> std::ptrdiff_t;
    auto send(const std::string &data) -> std::ptrdiff_t;

    // Server-side methods
    auto bind(std::uint16_t port,
              const std::string &address = "0.0.0.0") -> bool;
    auto listen(int backlog = 1) -> bool;
    auto accept() -> Socket;
    auto recv(void *buffer, std::size_t len) -> std::ptrdiff_t;
    auto recv(std::string &out, std::size_t max_len) -> std::ptrdiff_t;

  private:
    explicit Socket(int fd, Type type);

    int sock_fd_;
    Type type_;
};

} // namespace net
