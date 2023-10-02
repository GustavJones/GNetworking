#include "Socket.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    GNetworking::Socket ClientSock(AF_INET, SOCK_STREAM, 0);
    ClientSock.Connect("10.0.0.150", 1111);

    ClientSock.Send("This is a test message");

    while (true)
    {
        std::string input;
        std::getline(std::cin, input);

        ClientSock.Send(input);
    }

    return 0;
}
