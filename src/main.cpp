#include "Socket.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    GNetworking::Socket ServerSock(AF_INET, SOCK_STREAM, 0);
    ServerSock.Bind("10.0.0.150", 1111);
    ServerSock.Listen();
    GNetworking::Socket ClientSock = *ServerSock.Accept();

    while (true)
    {
        std::string data = ClientSock.Recv();
        std::cout << data << '\n';

        if (data == "--help\r\n")
        {
            ClientSock.Send("This is the help message: ");
        }

        if (data == "")
        {
            break;
        }
    }

    return 0;
}
