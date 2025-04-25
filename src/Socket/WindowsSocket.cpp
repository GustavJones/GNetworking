#if defined _WIN32
#include "Server/Socket.hpp"
#include <WS2tcpip.h>
#include <WinSock2.h>

namespace Wepp {
int SocketSetup(WeppSocket &_socket, const std::string &_address,
                const uint16_t _port) {
  int status;
  WSADATA wsaData;
  status = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (status != 0) {
    return status;
  }

  addrinfo *result = nullptr, hints;
  ZeroMemory(&hints, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  status = getaddrinfo(_address.c_str(), std::to_string(_port).c_str(), &hints, &result);
  if (status != 0) {
    SocketCleanup(_socket);
    return status;
  }

  _socket = WeppInvalidSocket;

  _socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (_socket == INVALID_SOCKET) {
    freeaddrinfo(result);
    SocketCleanup(_socket);
    return 1;
  }

  status = bind(_socket, result->ai_addr, (int)result->ai_addrlen);
  if (status == SOCKET_ERROR) {
    freeaddrinfo(result);
    SocketCleanup(_socket);
    return 1;
  }

  freeaddrinfo(result);

  if (listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
    SocketCleanup(_socket);
    return 1;
  }

  return 0;
}

int SocketCleanup(WeppSocket &_socket) {
  int status = WSACleanup();

  if (status != 0) {
    return status;
  }

  return 0;
}

WeppSocket SocketAccept(const WeppSocket &_socket) {
  WeppSocket client = WeppInvalidSocket;

  client = accept(_socket, NULL, NULL);
  return client;
}

int SocketClose(const WeppSocket &_socket) {
  return closesocket(_socket);
}

int SocketRecv(const WeppSocket &_socket, char *_buffer, const size_t _bufferLen, const WeppSocketFlags _flags) {
  return recv(_socket, _buffer, _bufferLen, _flags);
}


int SocketSend(const WeppSocket &_socket, const char *_buffer, const size_t _bufferLen, const WeppSocketFlags _flags) {
  return send(_socket, _buffer, _bufferLen, _flags);
}


bool SocketPoll(const WeppSocket &_socket, const WeppPollEvents _events) {
  WSAPOLLFD pollStruct;
  pollStruct.fd = _socket;
  pollStruct.events = 0;
  pollStruct.revents = 0;

  if (_events & WeppPOLLHUP) {
    pollStruct.events |= WeppPOLLHUP;
  }

  pollStruct.events ^= _events;

  if (WSAPoll(&pollStruct, 1, 0) < 0) {
    return false;
  }

  if (pollStruct.revents & _events) {
    return true;
  }
  else {
    return false;
  }

}

} // namespace Wepp
#endif // _WIN32
