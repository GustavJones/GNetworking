#include "GNetworking/Socket.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char const *argv[])
{
  int state = 1;
  std::string buff;

  GNetworking::Socket ServerSock;
  GNetworking::Socket ClientSock;

  if (ServerSock.CreateSocket(AF_INET, SOCK_STREAM, 0) < 0)
  {
    std::cerr << "Unable to Create Socket" << std::endl;
    std::exit(1);
  }

  if (ServerSock.Bind("0.0.0.0", 1111) < 0)
  {
    std::cerr << "Unable to Bind Server Socket" << std::endl;
    std::exit(1);
  }

  if (ServerSock.Listen() < 0)
  {
    std::cerr << "Unable to Listen on binded address" << std::endl;
    std::exit(1);
  }

  if (ServerSock.Accept(ClientSock) < 0)
  {
    std::cerr << "Unable To Connect to Client Socket" << std::endl;
  }

  while (state > 0)
  {
    state = ClientSock.Recv(buff);
    if (state > 0)
    {
      std::cout << buff << std::endl;
    }
  }

  return 0;
}
