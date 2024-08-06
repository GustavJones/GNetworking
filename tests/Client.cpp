#include "GNetworking/Socket.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char const *argv[]) {
  GNetworking::Socket ClientSock;
  if (ClientSock.CreateSocket(AF_INET, SOCK_STREAM, 0) < 0) {
    std::cerr << "Failed to Create socket" << std::endl;
    std::exit(1);
  }

  if (ClientSock.Connect("0.0.0.0", 1111) < 0) {
    std::cerr << "Failed to connect to socket" << std::endl;
    std::exit(1);
  }

  while (true) {
    std::string input;
    std::getline(std::cin, input);

    ClientSock.Send(input);
  }

  return 0;
}
