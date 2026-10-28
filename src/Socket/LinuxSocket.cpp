#include <cstddef>
#include <cstdint>
#include <vector>
#if defined __linux__
#include "GNetworking/Socket.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define SOCKET(sock) (int)sock
#define AF(addressFamily) (int)addressFamily
#define SOCK(socketType) (int)socketType
#define PROTO(socketProtocol) (int)socketProtocol
#define LEVEL(socketLevel) (int)socketLevel
#define OPTION(socketOption) (int)socketOption
#define SHUTDOWN(socketShutdownFlags) (int)socketShutdownFlags

namespace GNetworking {
int SocketSetup() {
	// Does nothing on Linux but is required for cross platform support
  return 0;
}

int SocketCleanup() {
	// Does nothing on Linux but is required for cross platform support
  return 0;
}

Socket SocketAccept(const Socket &_socket) {
  Socket client = Socket::Invalid;

  client = (Socket)accept(SOCKET(_socket), nullptr, nullptr);
  return client;
}

int SocketBind(const Socket &_socket, const std::string &_address, const uint16_t _port) {
  addrinfo *result = nullptr;
  int status = getaddrinfo(_address.c_str(), std::to_string(_port).c_str(), nullptr, &result);

  if (status != 0) {
    return status;
  }

  status = bind(SOCKET(_socket), result->ai_addr, (int)result->ai_addrlen);
  freeaddrinfo(result);
  return status;
}

int SocketListen(const Socket &_socket) {
  return listen(SOCKET(_socket), SOMAXCONN);
}

int SocketConnect(const Socket &_socket, const std::string &_address, const uint16_t _port) {
  addrinfo *result = nullptr;
  int status = getaddrinfo(_address.c_str(), std::to_string(_port).c_str(), nullptr, &result);

  if (status != 0)
  {
    return status;
  }

  status = connect(SOCKET(_socket), result->ai_addr, (int)result->ai_addrlen);
  return status;
}

Socket SocketCreate(const SocketAddressFamily _family, const SocketType _type, const SocketProtocol _proto) {
  return (Socket)socket(AF(_family), SOCK(_type), PROTO(_proto));
}

int SocketShutdown(const Socket &_socket, const SocketShutdownFlags _flags) {
  return shutdown(SOCKET(_socket), SHUTDOWN(_flags));
}

int SocketClose(const Socket &_socket) {
  return close(SOCKET(_socket));
}

int SocketRecv(const Socket &_socket, char *_buffer, const size_t _bufferLen, const SocketFlagsBitMask _flags) {
  return recv(SOCKET(_socket), _buffer, _bufferLen, _flags);
}


int SocketPeek(const Socket &_socket, char *_buffer, const size_t _bufferLen, const SocketFlagsBitMask _flags) {
  return SocketRecv(_socket, _buffer, _bufferLen, SocketFlag::Peek | (SocketFlag)(int)_flags);
}


int SocketSend(const Socket &_socket, const char *_buffer, const size_t _bufferLen, const SocketFlagsBitMask _flags) {
  return send(SOCKET(_socket), _buffer, _bufferLen, _flags);
}


bool SocketPoll(const Socket &_socket, const PollEventsBitMask _events) {
  pollfd pollStruct;
  pollStruct.fd = SOCKET(_socket);
  pollStruct.events = 0;
  pollStruct.revents = 0;

  if ((_events & (PollEventsBitMask)GNetworking::PollEvent::PollHup)) {
    pollStruct.events |= (PollEventsBitMask)GNetworking::PollEvent::PollHup;
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

size_t SocketPollSize(const Socket &_socket) {
  constexpr const size_t BUFFER_START_SIZE = 512;
  constexpr const size_t BUFFER_GROWTH_INCREMENT = 2;

  int output = -1;

  std::vector<char> buffer(BUFFER_START_SIZE);

  while (!SocketPoll(_socket, PollEvent::PollHup) && SocketPoll(_socket, PollEvent::PollIn)) {
    output = SocketPeek(_socket, buffer.data(), buffer.size(), SocketFlag::None);

    if (output >= buffer.size()) {
      buffer.resize(buffer.size() * BUFFER_GROWTH_INCREMENT);
    }
    else {
      break;
    } 
  }

  if (output < 0) {
    output = 0;
  }

  return output;
}

int SocketSetOption(const Socket &_socket, const SocketLevel _level, const SocketOption _option, const SocketSettings &_value) {
	const void *value = nullptr;
	size_t valueSize = 0;

  return setsockopt(SOCKET(_socket), LEVEL(_level), OPTION(_option), _value.Settings(), _value.SizeOfSettings());
}

} // namespace Wepp
#endif // __linux__
