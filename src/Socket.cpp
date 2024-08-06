#include "GNetworking/Socket.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace GNetworking {
// -------------------------------------
// ------------- Linux -----------------
// -------------------------------------
#ifdef __unix__

Socket::Socket() : m_init(false) {}

int Socket::SetSocket(const SOCKET &_sock, const int &_family, const int &_type,
                      const int &_protocol) {
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

int Socket::CreateSocket(const int &_family, const int &_type,
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

int Socket::Bind(std::string _ip, int _port) {
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

int Socket::Listen(const unsigned int &_connectionsNum) {
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

int Socket::Accept(Socket &_connectedSock) {
  if (!m_init) {
    return sock;
  }

  SOCKET sockReturn = accept(sock, nullptr, nullptr);
  int state =
      _connectedSock.SetSocket(sockReturn, m_family, m_type, m_protocol);

  return state;
}

int Socket::Accept(Socket &_connectedSock, const std::string &_addr,
                   const int &_port) {
  if (!m_init) {
    return sock;
  }

  sockaddr_in addr_in;
  addr_in.sin_addr.s_addr = inet_addr(_addr.c_str());
  addr_in.sin_port = htons(_port);
  addr_in.sin_family = m_family;

  SOCKET sockReturn =
      accept(sock, (sockaddr *)&addr_in, (socklen_t *)sizeof(addr_in));
  int state =
      _connectedSock.SetSocket(sockReturn, m_family, m_type, m_protocol);

  return state;
}

int Socket::Send(std::string _msg, int flags) {
  if (!m_init) {
    return sock;
  }

  int state = send(sock, _msg.c_str(), _msg.length(), flags);
  return state;
}

int Socket::Send(char *_msg, int _len, int flags) {
  if (!m_init) {
    return sock;
  }

  int state = send(sock, _msg, _len, flags);
  return state;
}

int Socket::Recv(std::string &buff, int _msgLen, int _flags) {
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

int Socket::Connect(std::string _ip, int _port) {
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

// -------------------------------------
// ------------ Windows ----------------
// -------------------------------------

#elif _WIN32

Socket::Socket(int _family, int _type, int _protocol)
    : m_family(_family), m_type(_type), m_protocol(_protocol) {
  WSAInit();

  sock = socket(m_family, m_type, m_protocol);
  if (sock == SOCKET_ERROR) {
    std::cerr << "Failed to create socket" << '\n';
  }
}

Socket::Socket(SOCKET _socket, int _family, int _type, int _protocol)
    : sock(_socket), m_family(_family), m_type(_type), m_protocol(_protocol) {
  if (sock == SOCKET_ERROR) {
    std::cerr << "Failed to create socket" << '\n';
  }
}

void Socket::Bind(std::string _ip, int _port) {
  sockaddr_in addr;
  addr.sin_family = m_family;
  addr.sin_addr.s_addr = inet_addr(_ip.c_str());
  addr.sin_port = htons(_port);

  if (bind(sock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
    std::cerr << "Failed to bind socket" << '\n';
  }
}

void Socket::Listen() {
  if (listen(sock, SOMAXCONN) == -1) {
    std::cerr << "Failed to start listening" << '\n';
  }
}

Socket *Socket::Accept() {
  SOCKET sockReturn = accept(sock, nullptr, nullptr);
  if (sockReturn == -1) {
    std::cerr << "Failed to accept connection" << '\n';
    return nullptr;
  }

  return new Socket(sockReturn, m_family, m_type, m_protocol);
}

Socket *Socket::Accept(sockaddr_in &_addr) {
  SOCKET sockReturn = accept(sock, (sockaddr *)&_addr, (int *)sizeof(_addr));
  if (sockReturn == -1) {
    std::cerr << "Failed to accept connection" << '\n';
    return nullptr;
  }

  return new Socket(sockReturn, m_family, m_type, m_protocol);
}

void Socket::Send(std::string _msg) {
  if (send(sock, _msg.c_str(), _msg.length(), 0) == -1) {
    std::cerr << "Failed to send data through socket" << '\n';
  }
}

void Socket::Send(char *_msg, int _len) {
  if (send(sock, _msg, _len, 0) == -1) {
    std::cerr << "Failed to send data through socket" << '\n';
  }
}

std::string Socket::Recv(int _msgLen) {
  std::string out;
  char *buffer = new char[_msgLen];
  int recvLen = recv(sock, buffer, _msgLen, 0);

  if (recvLen == -1) {
    std::cerr << "Failed to recieve data through socket" << '\n';
    return "";
  } else {
    out = buffer;
    if (out.length() > recvLen) {
      out = "";
      for (int i = 0; i < recvLen; i++) {
        out += buffer[i];
      }
    }
  }

  delete buffer;

  return out;
}

void Socket::Connect(std::string _ip, int _port) {
  sockaddr_in addr;
  addr.sin_family = m_family;
  addr.sin_addr.s_addr = inet_addr(_ip.c_str());
  addr.sin_port = htons(_port);

  if (connect(sock, (sockaddr *)&addr, sizeof(addr)) == -1) {
    std::cerr << "Failed to connect to remote host" << '\n';
  }
}

void Socket::Close() { closesocket(sock); }

Socket::~Socket() {
  this->Close();
  WSAEnd();
}

#endif

} // namespace GNetworking
