#pragma once
#include <cstddef>
#include <string>
#include <cstdint>

#ifdef _WIN32
#include <WinSock2.h>
#endif // _WIN32

namespace GNetworking {
#if defined _WIN32

typedef SOCKET GNetworkingSocket;
typedef int GNetworkingSocketAddressFamily;
typedef int GNetworkingSocketType;
typedef int GNetworkingSocketProtocol;
typedef int GNetworkingSocketFlags;
typedef short GNetworkingPollEvents;

#define GNetworkingInvalidSocket INVALID_SOCKET
#define GNetworkingPOLLIN POLLIN
#define GNetworkingPOLLOUT POLLOUT
#define GNetworkingPOLLHUP POLLHUP

#elif defined __linux__

typedef int GNetworkingSocket;
typedef int GNetworkingSocketFlags;
#define GNetworkingInvalidSocket -1;

#endif // _WIN32

int SocketSetup();
int SocketCleanup();

[[nodiscard]]
GNetworkingSocket SocketAccept(const GNetworkingSocket &_socket);
int SocketBind(const GNetworkingSocket &_socket, const std::string &_address, const uint16_t _port);
int SocketListen(const GNetworkingSocket &_socket);
int SocketConnect(const GNetworkingSocket &_socket, const std::string &_address, const uint16_t _port);

[[nodiscard]]
GNetworkingSocket SocketCreate(const GNetworkingSocketAddressFamily _family, const GNetworkingSocketType _type, const GNetworkingSocketProtocol _proto);
int SocketShutdown(const GNetworkingSocket &_socket, const GNetworkingSocketFlags _flags);
int SocketClose(const GNetworkingSocket &_socket);

int SocketRecv(const GNetworkingSocket &_socket, char *_buffer, const size_t _bufferLen, const GNetworkingSocketFlags _flags);
int SocketSend(const GNetworkingSocket &_socket, const char *_buffer, const size_t _bufferLen, const GNetworkingSocketFlags _flags);

[[nodiscard]]
bool SocketPoll(const GNetworkingSocket &_socket, const GNetworkingPollEvents _events);
} // namespace Wepp
