#include "GNetworking/TCP/Server.hpp"
#include <chrono>
#include <thread>

namespace GNetworking {
namespace TCP {
void Server::AcceptThread() {
  while (m_acceptingConnections) {
    if (SocketPoll(m_serverSocket, PollEvent::PollIn)) {
      GNetworking::Socket sock = SocketAccept(m_serverSocket);
      std::lock_guard<std::mutex> lock(m_connectionsMutex);
      m_connections.push_back(sock);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

Server::Server(const std::string &_bind, const uint16_t _port) {
  SocketSetup();
  m_serverSocket = SocketCreate(SocketAddressFamily::INET, SocketType::Stream,
                                SocketProtocol::TCP);
  GNetworking::ReuseAddrSettings reuseSettings;
  reuseSettings.SetEnabled(1);
  SocketSetOption(m_serverSocket, SocketLevel::Socket, SocketOption::ReuseAddr,
                  reuseSettings);

  SocketBind(m_serverSocket, _bind, _port);
  SocketListen(m_serverSocket);
};

Server::~Server() {
  StopAcceptingConnections();
  SocketShutdown(m_serverSocket, SocketShutdownFlags::RDWR);
  SocketClose(m_serverSocket);
  SocketCleanup();
};

void Server::AcceptConnections() {
  m_acceptingConnections = true;
  m_acceptingThread = std::async(std::launch::async,
                                 &GNetworking::TCP::Server::AcceptThread, this);
}

void Server::StopAcceptingConnections() { m_acceptingConnections = false; }

bool Server::HasActiveConnections() const {
  std::lock_guard<std::mutex> lock(m_connectionsMutex);
  if (m_connections.size() > 0)
    return true;
  else
    return false;
}

GNetworking::TCP::Connection Server::GetConnection() {
  GNetworking::TCP::Connection output;

  std::lock_guard<std::mutex> lock(m_connectionsMutex);
  if (m_connections.size() > 0) {
    output = std::move(m_connections.back());
    m_connections.pop_back();
  }

  return output;
}
} // namespace TCP
} // namespace GNetworking
