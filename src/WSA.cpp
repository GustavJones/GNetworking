#include "WSA.h"
#ifdef _WIN32
#include <winsock2.h>
#include <iostream>

namespace GNetworking
{
    void WSAInit()
    {
        WSAData wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "Failed to load Winsock2" << '\n';
            exit(1);
        }
    }

    void WSAEnd()
    {
        WSACleanup();
    }

} // namespace GNetworking
#endif