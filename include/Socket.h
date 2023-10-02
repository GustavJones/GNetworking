#pragma once
#include <string>

#ifdef unix
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef int SOCKET;

#elif _WIN32
#include <iostream>
#include <winsock2.h>
#include "WSA.h"

#endif

namespace GNetworking
{
    class Socket
    {
    private:
        int m_family, m_type, m_protocol;
        Socket *clientSock;

    public:
        SOCKET sock;

        Socket(SOCKET _family, int _type, int _protocol);
        Socket(SOCKET _socket, int _family, int _type, int _protocol);

        void Bind(std::string _ip, int _port);
        void Listen();

        Socket *Accept();
        Socket *Accept(sockaddr_in &_addr);

        void Send(std::string _msg);
        std::string Recv(int _msgLen = 1024);

        void Connect(std::string _ip, int _port);

        ~Socket();
    };
} // namespace GNetworking
