#pragma once
#include <csignal>
#include <cstddef>
#include <string>
#include <cstdint>

#if defined _WIN32
#include <WinSock2.h>
#elif defined __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#endif // _WIN32

namespace GNetworking {
typedef int GNetworkingSocketLevel;
typedef int GNetworkingSocketOption;
typedef const void * GNetworkingSocketValue;
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
#define GNetworkingSHUTDOWNRD SD_RECEIVE
#define GNetworkingSHUTDOWNWR SD_SEND
#define GNetworkingSHUTDOWNRDWR SD_BOTH

#elif defined __linux__
typedef int GNetworkingSocket;
typedef int GNetworkingSocketAddressFamily;
typedef int GNetworkingSocketType;
typedef int GNetworkingSocketProtocol;
typedef int GNetworkingSocketFlags;
typedef short GNetworkingPollEvents;

#define GNetworkingInvalidSocket -1
#define GNetworkingPOLLIN POLLIN
#define GNetworkingPOLLOUT POLLOUT
#define GNetworkingPOLLHUP POLLHUP
#define GNetworkingSHUTDOWNRD SHUT_RD
#define GNetworkingSHUTDOWNWR SHUT_WR
#define GNetworkingSHUTDOWNRDWR SHUT_RDWR

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

int SocketSetOption(const GNetworkingSocket &_socket, const GNetworkingSocketLevel _level, const GNetworkingSocketOption _option, GNetworkingSocketValue _value, const size_t _valueLen);
} // namespace Wepp
