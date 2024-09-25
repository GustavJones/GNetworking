#pragma once

#ifdef _MSC_VER

// #pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>

namespace GNetworking
{
    void WSAInit();
    void WSAEnd();
} // namespace GNetworking

#endif