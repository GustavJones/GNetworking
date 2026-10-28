#pragma once
#include "GNetworking/TCP/Connection.hpp"

namespace GNetworking {
namespace TCP {
class Client : public Connection {
public:
  Client(const std::string &_connect, const uint16_t _port);

  ~Client();

  Client(const Client &_copy) = delete;
  Client(Client &&_move) = delete;
  Client &operator=(const Client &_copy) = delete;
  Client &operator=(Client &&_move) = delete;
};
} // namespace TCP
} // namespace GNetworking
