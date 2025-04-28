#if defined __linux__
#include "GNetworking/Socket.hpp"
#include <sys/types.h>
#include <netdb.h>

namespace GNetworking {
int SocketSetup() {
  return 0;
}

int SocketCleanup() {
  return 0;
}

GNetworkingSocket SocketAccept(const GNetworkingSocket &_socket) {
  GNetworkingSocket client = GNetworkingInvalidSocket;

  client = accept(_socket, NULL, NULL);
  return client;
}

int SocketBind(const GNetworkingSocket &_socket, const std::string &_address, const uint16_t _port) {
  addrinfo *result = nullptr;
  int status = getaddrinfo(_address.c_str(), std::to_string(_port).c_str(), nullptr, &result);

  if (status != 0) {
    return status;
  }

  status = bind(_socket, result->ai_addr, (int)result->ai_addrlen);
  freeaddrinfo(result);
  return status;
}

int SocketListen(const GNetworkingSocket &_socket) {
  return listen(_socket, SOMAXCONN);
}

int SocketConnect(const GNetworkingSocket &_socket, const std::string &_address, const uint16_t _port) {
  addrinfo *result = nullptr;
  int status = getaddrinfo(_address.c_str(), std::to_string(_port).c_str(), nullptr, &result);

  if (status != 0)
  {
    return status;
  }

  status = connect(_socket, result->ai_addr, (int)result->ai_addrlen);
  return status;
}

GNetworkingSocket SocketCreate(const GNetworkingSocketAddressFamily _family, const GNetworkingSocketType _type, const GNetworkingSocketProtocol _proto) {
  return socket(_family, _type, _proto);
}

int SocketShutdown(const GNetworkingSocket &_socket, const GNetworkingSocketFlags _flags) {
  return shutdown(_socket, _flags);
}

int SocketClose(const GNetworkingSocket &_socket) {
  return close(_socket);
}

int SocketRecv(const GNetworkingSocket &_socket, char *_buffer, const size_t _bufferLen, const GNetworkingSocketFlags _flags) {
  return recv(_socket, _buffer, _bufferLen, _flags);
}


int SocketSend(const GNetworkingSocket &_socket, const char *_buffer, const size_t _bufferLen, const GNetworkingSocketFlags _flags) {
  return send(_socket, _buffer, _bufferLen, _flags);
}


bool SocketPoll(const GNetworkingSocket &_socket, const GNetworkingPollEvents _events) {
  pollfd pollStruct;
  pollStruct.fd = _socket;
  pollStruct.events = 0;
  pollStruct.revents = 0;

  if (_events & GNetworkingPOLLHUP) {
    pollStruct.events |= GNetworkingPOLLHUP;
  }

  pollStruct.events ^= _events;

  if (poll(&pollStruct, 1, 0) < 0) {
    return false;
  }

  if (pollStruct.revents & _events) {
    return true;
  }
  else {
    return false;
  }

}

int SocketSetOption(const GNetworkingSocket &_socket, const GNetworkingSocketLevel _level, const GNetworkingSocketOption _option, GNetworkingSocketValue _value, const size_t _valueLen) {
  return setsockopt(_socket, _level, _option, _value, _valueLen);
}

} // namespace Wepp
#endif // __linux__
