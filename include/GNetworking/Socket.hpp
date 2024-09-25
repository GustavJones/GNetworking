#pragma once
#include <string>

#ifdef __unix__
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef int SOCKET;

#elif _WIN32
#include "WSA.hpp"
#include <iostream>
#include <winsock2.h>

#endif

namespace GNetworking
{
  class Socket
  {
  private:
    int m_family, m_type, m_protocol;
    bool m_init = false;

  public:
    SOCKET sock;

    Socket();
    Socket(const int &_family, const int &_type, const int &_protocol);

    SOCKET SetSocket(const SOCKET &_sock, const int &_family, const int &_type, const int &_protocol);
    SOCKET CreateSocket(const int &_family, const int &_type, const int &_protocol);

    int Bind(std::string _ip, int _port);
    int Listen(const unsigned int &_connectionsNum = 0);

    SOCKET Accept(Socket &_connectedSock);
    SOCKET Accept(Socket &_connectedSock, const std::string &_addr, const int &_port);

    int Send(std::string _msg, int flags = 0) const;
    int Send(char *_msg, int _len, int flags = 0) const;
    int Recv(std::string &buff, int _msgLen = 1024, int _flags = 0) const;
    int Recv(char *buff, int _msgLen = 1024, int _flags = 0) const;

    int Connect(std::string _ip, int _port);
    void Close();

    ~Socket();
  };
} // namespace GNetworking
