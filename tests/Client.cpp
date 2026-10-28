#include "GNetworking/TCP/Client.hpp"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, const char *argv[]) {
  std::vector<char> buffer;
  std::string send = "Hello from the server side!";
  buffer.insert(buffer.end(), send.begin(), send.end());

  GNetworking::TCP::Client client("127.0.0.1", 8080);
  client.Send(buffer);

  bool ok;
  do {
    ok = client.Recieve(buffer);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } while (ok && buffer.size() == 0);

  std::cout << buffer.size() << std::endl;
  if (buffer.size() > 0)
    std::cout << buffer.data() << std::endl;
}
