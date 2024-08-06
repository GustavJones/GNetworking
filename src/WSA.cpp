#include "GNetworking/WSA.hpp"
#ifdef _WIN32
#include <iostream>
#include <winsock2.h>

namespace GNetworking {
void WSAInit() {
  WSAData wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "Failed to load Winsock2" << '\n';
    exit(1);
  }
}

void WSAEnd() { WSACleanup(); }

} // namespace GNetworking
#endif
