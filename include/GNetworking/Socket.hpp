#pragma once
#include <cstddef>
#include <string>
#include <cstdint>

#ifdef _WIN32
#include <WinSock2.h>
#endif // _WIN32

namespace Wepp {
#if defined _WIN32

typedef SOCKET WeppSocket;
typedef int WeppSocketAddressFamily;
typedef int WeppSocketType;
typedef int WeppSocketProtocol;
typedef int WeppSocketFlags;
typedef short WeppPollEvents;

#define WeppInvalidSocket INVALID_SOCKET
#define WeppPOLLIN POLLIN
#define WeppPOLLOUT POLLOUT
#define WeppPOLLHUP POLLHUP

#elif defined __linux__

typedef int WeppSocket;
typedef int WeppSocketFlags;
#define WeppInvalidSocket -1;

#endif // _WIN32

int SocketSetup();
int SocketCleanup();

[[nodiscard]]
WeppSocket SocketAccept(const WeppSocket &_socket);

[[nodiscard]]
WeppSocket SocketCreate();
int SocketShutdown(const WeppSocket &_socket);
int SocketClose(const WeppSocket &_socket);

int SocketRecv(const WeppSocket &_socket, char *_buffer, const size_t _bufferLen, const WeppSocketFlags _flags);
int SocketSend(const WeppSocket &_socket, const char *_buffer, const size_t _bufferLen, const WeppSocketFlags _flags);

[[nodiscard]]
bool SocketPoll(const WeppSocket &_socket, const WeppPollEvents _events);
} // namespace Wepp
