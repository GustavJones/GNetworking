// -------------------------------------
// ------------- Linux -----------------
// -------------------------------------
#ifdef __GNUC__

#include "GNetworking/Socket.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace GNetworking {
Socket::Socket() : m_init(false) {}

SOCKET Socket::SetSocket(const SOCKET &_sock, const int &_family,
                         const int &_type, const int &_protocol) {
  if (_sock > 0) {
    m_family = _family;
    m_type = _type;
    m_protocol = _protocol;
    sock = _sock;
    m_init = true;
    return _sock;
  } else {
    return _sock;
  }
}

SOCKET Socket::CreateSocket(const int &_family, const int &_type,
                            const int &_protocol) {
  m_family = _family;
  m_type = _type;
  m_protocol = _protocol;

  sock = socket(m_family, m_type, m_protocol);

  if (sock > 0) {
    m_init = true;
  }
  return sock;
}

Socket::Socket(const int &_family, const int &_type, const int &_protocol) {
  CreateSocket(_family, _type, _protocol);
}

int Socket::Bind(std::string _ip, int _port) const {
  if (!m_init) {
    return sock;
  }
  sockaddr_in addr;
  addr.sin_family = m_family;
  addr.sin_addr.s_addr = inet_addr(_ip.c_str());
  addr.sin_port = htons(_port);

  int state = bind(sock, (sockaddr *)&addr, (socklen_t)sizeof(addr));
  return state;
}

int Socket::Listen(const unsigned int &_connectionsNum) const {
  if (!m_init) {
    return sock;
  }

  int state;
  if (_connectionsNum == 0) {
    state = listen(sock, SOMAXCONN);
  } else {
    state = listen(sock, _connectionsNum);
  }
  return state;
}

SOCKET Socket::Accept(Socket &_connectedSock) const {
  if (!m_init) {
    return sock;
  }

  SOCKET sockReturn = accept(sock, nullptr, nullptr);
  SOCKET state =
      _connectedSock.SetSocket(sockReturn, m_family, m_type, m_protocol);

  return state;
}

SOCKET Socket::Accept(Socket &_connectedSock, const std::string &_addr,
                      const int &_port) const {
  if (!m_init) {
    return sock;
  }

  sockaddr_in addr_in;
  addr_in.sin_addr.s_addr = inet_addr(_addr.c_str());
  addr_in.sin_port = htons(_port);
  addr_in.sin_family = m_family;

  SOCKET sockReturn =
      accept(sock, (sockaddr *)&addr_in, (socklen_t *)sizeof(addr_in));
  SOCKET state =
      _connectedSock.SetSocket(sockReturn, m_family, m_type, m_protocol);

  return state;
}

int Socket::Send(std::string _msg, int flags) const {
  if (!m_init) {
    return sock;
  }

  int state = send(sock, _msg.c_str(), _msg.length(), flags);
  return state;
}

int Socket::Send(char *_msg, int _len, int flags) const {
  if (!m_init) {
    return sock;
  }

  int state = send(sock, _msg, _len, flags);
  return state;
}

int Socket::Recv(std::string &buff, int _msgLen, int _flags) const {
  if (!m_init) {
    return sock;
  }

  char *cbuffer = new char[_msgLen];
  int state = recv(sock, cbuffer, _msgLen, _flags);

  if (state > 0) {
    buff = "";
    for (int i = 0; i < state; i++) {
      buff += cbuffer[i];
    }
  }

  delete[] cbuffer;
  return state;
}

int Socket::Recv(char *buff, int _msgLen, int _flags) const {
  if (!m_init) {
    return sock;
  }

  int state = recv(sock, buff, _msgLen, _flags);
  return state;
}

int Socket::Connect(std::string _ip, int _port) const {
  if (!m_init) {
    return sock;
  }

  sockaddr_in addr;
  addr.sin_family = m_family;
  addr.sin_addr.s_addr = inet_addr(_ip.c_str());
  addr.sin_port = htons(_port);

  int state = connect(sock, (sockaddr *)&addr, (socklen_t)sizeof(addr));
  return state;
}

void Socket::Close() {
  close(sock);
  m_init = false;
}

Socket::~Socket() { this->Close(); }
} // namespace GNetworking

// -------------------------------------
// ------------ Windows ----------------
// -------------------------------------

#elif _MSC_VER

#include "GNetworking/Socket.hpp"
#include "GNetworking/WSA.hpp"

namespace GNetworking {

Socket::Socket() : m_init(false) {
  // GNetworking::WSAInit();
}

Socket::Socket(const int &_family, const int &_type, const int &_protocol) {
  CreateSocket(_family, _type, _protocol);
}

SOCKET Socket::SetSocket(const SOCKET &_sock, const int &_family,
                         const int &_type, const int &_protocol) {
  if (_sock != SOCKET_ERROR) {
    m_family = _family;
    m_type = _type;
    m_protocol = _protocol;
    sock = _sock;
    m_init = true;
    return _sock;
  } else {
    return _sock;
  }
}

SOCKET Socket::CreateSocket(const int &_family, const int &_type,
                            const int &_protocol) {
  m_family = _family;
  m_type = _type;
  m_protocol = _protocol;

  sock = socket(m_family, m_type, m_protocol);

  if (sock != SOCKET_ERROR) {
    m_init = true;
  }

  return sock;
}

int Socket::Bind(std::string _ip, int _port) const {
  if (!m_init) {
    return sock;
  }

  struct sockaddr_in addr;
  addr.sin_family = m_family;
  addr.sin_addr.s_addr = inet_addr(_ip.c_str());
  addr.sin_port = htons(_port);

  int state = bind(sock, (sockaddr *)&addr, sizeof(addr));
  return state;
}

int Socket::Listen(const unsigned int &_connectionsNum) const {
  if (!m_init) {
    return sock;
  }

  int state;
  if (_connectionsNum == 0) {
    state = listen(sock, SOMAXCONN);
  } else {
    state = listen(sock, _connectionsNum);
  }
  return state;
}

SOCKET Socket::Accept(Socket &_connectedSock) const {
  if (!m_init) {
    return sock;
  }

  SOCKET sockReturn = accept(sock, nullptr, nullptr);
  SOCKET state =
      _connectedSock.SetSocket(sockReturn, m_family, m_type, m_protocol);

  return state;
}

SOCKET Socket::Accept(Socket &_connectedSock, const std::string &_addr,
                      const int &_port) const {
  if (!m_init) {
    return sock;
  }

  sockaddr_in addr_in;
  addr_in.sin_addr.s_addr = inet_addr(_addr.c_str());
  addr_in.sin_port = htons(_port);
  addr_in.sin_family = m_family;

  SOCKET sockReturn =
      accept(sock, (sockaddr *)&addr_in, (int *)sizeof(addr_in));
  SOCKET state =
      _connectedSock.SetSocket(sockReturn, m_family, m_type, m_protocol);

  return state;
}

int Socket::Send(std::string _msg, int flags) const {
  if (!m_init) {
    return sock;
  }

  int state = send(sock, _msg.c_str(), _msg.length(), flags);
  return state;
}

int Socket::Send(char *_msg, int _len, int flags) const {
  if (!m_init) {
    return sock;
  }

  int state = send(sock, _msg, _len, flags);
  return state;
}

int Socket::Recv(std::string &buff, int _msgLen, int _flags) const {
  if (!m_init) {
    return sock;
  }

  char *cbuffer = new char[_msgLen];
  int state = recv(sock, cbuffer, _msgLen, _flags);

  if (state > 0) {
    buff = "";
    for (int i = 0; i < state; i++) {
      buff += cbuffer[i];
    }
  }

  delete[] cbuffer;
  return state;
}

int Socket::Recv(char *buff, int _msgLen, int _flags) const {
  if (!m_init) {
    return sock;
  }

  int state = recv(sock, buff, _msgLen, _flags);
  return state;
}

int Socket::Connect(std::string _ip, int _port) const {
  if (!m_init) {
    return sock;
  }

  sockaddr_in addr;
  addr.sin_family = m_family;
  addr.sin_addr.s_addr = inet_addr(_ip.c_str());
  addr.sin_port = htons(_port);

  int state = connect(sock, (sockaddr *)&addr, sizeof(addr));
  return state;
}

void Socket::Close() {
  closesocket(sock);
  m_init = false;
}

Socket::~Socket() {
  Close();
  GNetworking::WSAEnd();
}
} // namespace GNetworking

#endif
