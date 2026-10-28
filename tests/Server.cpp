#include "GNetworking/TCP/Server.hpp"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, const char *argv[]) {
  std::vector<char> buffer;
  std::string send = "Hello from the server side!";

  GNetworking::TCP::Server server("0.0.0.0", 8080);
  server.AcceptConnections();

  while (true) {
    while (!server.HasActiveConnections()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Found a connection!" << std::endl;

    GNetworking::TCP::Connection clientConnection = server.GetConnection();
    if (!clientConnection.IsValid())
      continue;

    std::cout << "Recieving data!" << std::endl;
    bool ok;

    do {
      ok = clientConnection.Recieve(buffer);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (ok && buffer.size() == 0);

    std::cout << buffer.size() << std::endl;

    if (buffer.size() > 0)
      std::cout << buffer.data() << std::endl;

    buffer.clear();
    buffer.insert(buffer.begin(), send.begin(), send.end());

    std::cout << "Sending data!" << std::endl;
    clientConnection.Send(buffer);

    std::cout << "Shutting down connection" << std::endl;
  }

  server.StopAcceptingConnections();
}
