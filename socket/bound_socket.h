#pragma once

#include <string>

#include "socket.h"

namespace net {

class ConnectedSocket; // Forward declaration

class BoundSocket {
  public:
    explicit BoundSocket(Socket &&socket, int backlog = 1);

    BoundSocket(const BoundSocket &) = delete;
    auto operator=(const BoundSocket &) -> BoundSocket & = delete;

    BoundSocket(BoundSocket &&other) noexcept;
    auto operator=(BoundSocket &&other) noexcept -> BoundSocket &;

    ~BoundSocket() = default;

    [[nodiscard]] auto is_valid() const -> bool;
    [[nodiscard]] auto fd() const -> int;
    auto close() -> void;

    auto listen(int backlog = 1) -> bool;
    auto accept() -> ConnectedSocket;

  private:
    Socket socket_;
};

} // namespace net
