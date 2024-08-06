#pragma once

#ifdef _WIN32

namespace GNetworking
{
    void WSAInit();
    void WSAEnd();
} // namespace GNetworking

#endif