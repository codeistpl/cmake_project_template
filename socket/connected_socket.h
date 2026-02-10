#pragma once

#include <cstddef>
#include <string>

#include "socket.h"

namespace net {

class ConnectedSocket {
  public:
    explicit ConnectedSocket(Socket &&socket);
    explicit ConnectedSocket(int fd, Socket::Type type) noexcept;

    ConnectedSocket(const ConnectedSocket &) = delete;
    auto operator=(const ConnectedSocket &) -> ConnectedSocket & = delete;

    ConnectedSocket(ConnectedSocket &&other) noexcept;
    auto operator=(ConnectedSocket &&other) noexcept -> ConnectedSocket &;

    ~ConnectedSocket() = default;

    [[nodiscard]] auto is_valid() const -> bool;
    [[nodiscard]] auto fd() const -> int;
    auto close() -> void;

    [[nodiscard]] auto send(const void *data,
                            std::size_t len) const -> std::ptrdiff_t;
    [[nodiscard]] auto send(const std::string &data) const -> std::ptrdiff_t;
    auto recv(void *buffer, std::size_t len) const -> std::ptrdiff_t;
    auto recv(std::string &out, std::size_t max_len) const -> std::ptrdiff_t;

  private:
    Socket socket_;
};

} // namespace net
