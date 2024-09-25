#pragma once

#ifdef _MSC_VER

#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace GNetworking
{
    void WSAInit();
    void WSAEnd();
} // namespace GNetworking

#endif