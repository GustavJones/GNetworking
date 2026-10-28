#include "GNetworking/TCP/Connection.hpp"

namespace GNetworking {
namespace TCP {
void Connection::SetSocket(const GNetworking::Socket _sock) {
  m_connectionSocket = _sock;
}
GNetworking::Socket Connection::GetSocket() const { return m_connectionSocket; }

Connection::Connection(const GNetworking::Socket _sock)
    : m_connectionSocket(_sock) {}

Connection::Connection(Connection &&_move)
    : m_connectionSocket(_move.m_connectionSocket) {
  _move.m_connectionSocket = GNetworking::Socket::Invalid;
}

Connection &Connection::operator=(Connection &&_move) {
  SocketClose(m_connectionSocket);
  SetSocket(_move.m_connectionSocket);
  _move.SetSocket(GNetworking::Socket::Invalid);

  return *this;
}

Connection::~Connection() {
  SocketShutdown(m_connectionSocket, SocketShutdownFlags::RDWR);
  SocketClose(m_connectionSocket);
  SetSocket(GNetworking::Socket::Invalid);
}

bool Connection::IsValid() const {
  if (m_connectionSocket == GNetworking::Socket::Invalid)
    return false;
  if (SocketPoll(m_connectionSocket,
                 PollEvent::PollNVal | PollEvent::PollHup | PollEvent::PollErr))
    return false;

  return true;
}

bool Connection::Recieve(std::vector<char> &_buffer) const {
  std::vector<char> temp(4096);
  _buffer.clear();

  while (
      SocketPoll(m_connectionSocket, PollEvent::PollIn | PollEvent::PollHup)) {
    auto recvCount = SocketRecv(m_connectionSocket, temp.data(), temp.size(),
                                SocketFlag::None);

    if (recvCount <= 0) {
      return false;
    }

    _buffer.insert(_buffer.end(), temp.begin(), temp.begin() + recvCount);
  }

  return true;
}

bool Connection::Send(const std::vector<char> &_buffer) const {
  if (!SocketPoll(m_connectionSocket, PollEvent::PollOut))
    return true;
  else if (SocketPoll(m_connectionSocket, PollEvent::PollHup))
    return true;

  auto sendCount = SocketSend(m_connectionSocket, _buffer.data(),
                              _buffer.size(), SocketFlag::None);

  if (sendCount < 0)
    return false;
  else
    return true;
}
} // namespace TCP
} // namespace GNetworking
