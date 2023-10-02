#include "Socket.h"

namespace GNetworking
{
#ifdef unix

    Socket::Socket(int _family, int _type, int _protocol) : m_family(_family), m_type(_type), m_protocol(_protocol)
    {
        sock = socket(m_family, m_type, m_protocol);
        if (sock == -1)
        {
            std::cerr << "Failed to create socket" << '\n';
        }
    }

    Socket::Socket(int _socket, int _family, int _type, int _protocol) : sock(_socket), m_family(_family), m_type(_type), m_protocol(_protocol)
    {
        if (sock == -1)
        {
            std::cerr << "Failed to create socket" << '\n';
        }
    }

    void Socket::Bind(std::string _ip, int _port)
    {
        sockaddr_in addr;
        addr.sin_family = m_family;
        addr.sin_addr.s_addr = inet_addr(_ip.c_str());
        addr.sin_port = htons(_port);

        if (bind(sock, (sockaddr *)&addr, (socklen_t)sizeof(addr)) == -1)
        {
            std::cerr << "Failed to bind socket" << '\n';
        }
    }

    void Socket::Listen()
    {
        if (listen(sock, SOMAXCONN) == -1)
        {
            std::cerr << "Failed to start listening" << '\n';
        }
    }

    Socket *Socket::Accept()
    {
        int sockReturn = accept(sock, nullptr, nullptr);
        if (sockReturn == -1)
        {
            std::cerr << "Failed to accept connection" << '\n';
            return nullptr;
        }

        clientSock = new Socket(sockReturn, m_family, m_type, m_protocol);
        return clientSock;
    }

    Socket *Socket::Accept(sockaddr_in &_addr)
    {
        int sockReturn = accept(sock, (sockaddr *)&_addr, (socklen_t *)sizeof(_addr));
        if (sockReturn == -1)
        {
            std::cerr << "Failed to accept connection" << '\n';
            return nullptr;
        }

        clientSock = new Socket(sockReturn, m_family, m_type, m_protocol);
        return clientSock;
    }

    void Socket::Send(std::string _msg)
    {
        if (send(sock, _msg.c_str(), _msg.length(), 0) == -1)
        {
            std::cerr << "Failed to send data through socket" << '\n';
        }
    }

    std::string Socket::Recv(int _msgLen)
    {
        std::string out;
        char buffer[_msgLen];
        int recvLen = recv(sock, buffer, _msgLen, 0);

        if (recvLen == -1)
        {
            std::cerr << "Failed to recieve data through socket" << '\n';
            return "";
        }
        else
        {
            out = buffer;
            if (out.length() > recvLen)
            {
                out = "";
                for (int i = 0; i < recvLen; i++)
                {
                    out += buffer[i];
                }
            }
        }

        return out;
    }

    void Socket::Connect(std::string _ip, int _port)
    {
        sockaddr_in addr;
        addr.sin_family = m_family;
        addr.sin_addr.s_addr = inet_addr(_ip.c_str());
        addr.sin_port = htons(_port);

        if (connect(sock, (sockaddr *)&addr, (socklen_t)sizeof(addr)) == -1)
        {
            std::cerr << "Failed to connect to remote host" << '\n';
        }
    }

    Socket::~Socket()
    {
        delete clientSock;
        close(sock);
    }

#endif

} // namespace GNetworking
