#pragma once
#include "GNetworking/Socket.hpp"
#include <vector>

namespace GNetworking {
namespace TCP {
class Server;

class Connection {
private:
  GNetworking::Socket m_connectionSocket = GNetworking::Socket::Invalid;

protected:
  void SetSocket(const GNetworking::Socket _sock);
  GNetworking::Socket GetSocket() const;

public:
  Connection() = default;
  Connection(const GNetworking::Socket _sock);

  Connection(const Connection &_copy) = delete;
  Connection &operator=(const Connection &_copy) = delete;
  Connection(Connection &&_move);
  Connection &operator=(Connection &&_move);

  ~Connection();

  bool IsValid() const;
  bool Recieve(std::vector<char> &_buffer) const;
  bool Send(const std::vector<char> &_buffer) const;
};
} // namespace TCP
} // namespace GNetworking
