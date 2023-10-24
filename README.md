# GNetworking

## What is it:
- A simple library to simplify network programming in C++.
- A cross-platform C++ socket library tested on Windows and Linux.
- A fun project for me to learn about networking

## How can I use it:
Include the socket header file `#include "Socket.h"` after linking and setting up include directories. Then create a socket
by calling `GNetworking::Socket` and creating a socket object. The socket object contains methods for Binding,
 Connecting, Listening, Sending and Recieving.

### Client:
```c++
#include "Socket.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    GNetworking::Socket ClientSock(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ClientSock.Connect("127.0.0.1", 8080);

    ClientSock.Send("This is a test message");

    while (true)
    {
        std::string input;
        std::getline(std::cin, input);

        ClientSock.Send(input);
    }

    return 0;
}
```

### Server
```c++
#include "Socket.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    GNetworking::Socket ServerSock(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ServerSock.Bind("127.0.0.1", 8080); // Input private ip to be able to port forward
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
```

#### Note:
- Information about socket family, type and protocol is available at 
[Winsock 2 Docs]([https://](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket)) 
which is also used for Linux code.
- I have not implemented support for IPv6 at the moment

## Usage:
Feel free to learn from my code and use snippets where needed. Please let me know where I can improve this 
small library to make it more simple to use.