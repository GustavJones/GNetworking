#pragma once
#include "GNetworking/Socket.hpp"
#include "GNetworking/TCP/Connection.hpp"
#include <atomic>
#include <future>
#include <mutex>
#include <string>

namespace GNetworking {
namespace TCP {
class Server {
private:
  GNetworking::Socket m_serverSocket = GNetworking::Socket::Invalid;
  std::future<void> m_acceptingThread;
  std::atomic<bool> m_acceptingConnections = false;
  mutable std::mutex m_connectionsMutex;
  std::vector<GNetworking::TCP::Connection> m_connections;

  void AcceptThread();

public:
  Server(const std::string &_bind, const uint16_t _port);

  ~Server();

  Server(const Server &_copy) = delete;
  Server(Server &&_move) = delete;
  Server &operator=(const Server &_copy) = delete;
  Server &operator=(Server &&_move) = delete;

  void AcceptConnections();
  void StopAcceptingConnections();
  bool HasActiveConnections() const;
  GNetworking::TCP::Connection GetConnection();
};
} // namespace TCP
} // namespace GNetworking
