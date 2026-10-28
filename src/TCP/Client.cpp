#include "GNetworking/TCP/Client.hpp"

namespace GNetworking {
namespace TCP {
Client::Client(const std::string &_connect, const uint16_t _port) {
  GNetworking::Socket client;
  SocketSetup();
  client = SocketCreate(SocketAddressFamily::INET, SocketType::Stream,
                        SocketProtocol::TCP);
  SocketConnect(client, _connect, _port);

  SetSocket(client);
}

Client::~Client() {
  std::vector<char> buffer(4);
  GNetworking::Socket client = GetSocket();
  SocketShutdown(client, SocketShutdownFlags::WR);
  SocketRecv(client, buffer.data(), buffer.size(), SocketFlag::None);
}
} // namespace TCP
} // namespace GNetworking
