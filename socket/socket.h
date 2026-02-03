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

    auto connect(const std::string &host, std::uint16_t port) -> bool;
    auto send(const void *data, std::size_t len) -> std::ptrdiff_t;
    auto send(const std::string &data) -> std::ptrdiff_t;

  private:
    int sock_fd_;
    Type type_;
};

} // namespace net
