#include "GNetworking/Socket.hpp"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, const char *argv[]) {
	std::vector<char> read;
	std::vector<char> buffer(4096);
	int output;
	GNetworking::ReuseAddrSettings reuseaddr {};
	reuseaddr.SetEnabled(1);

	GNetworking::SocketSetup();

	GNetworking::Socket server = GNetworking::SocketCreate(GNetworking::SocketAddressFamily::INET, GNetworking::SocketType::Stream, GNetworking::SocketProtocol::Default);
	GNetworking::SocketSetOption(server, GNetworking::SocketLevel::Socket, GNetworking::SocketOption::ReuseAddr, reuseaddr);
	GNetworking::SocketBind(server, "0.0.0.0", 8080);
	GNetworking::SocketListen(server);

	std::cout << "Listening on server socket..." << std::endl;
	while (true) {
		read.clear();

		if (GNetworking::SocketPoll(server, GNetworking::PollEvent::PollIn)) {
			std::cout << "Connection found..." << std::endl;
			GNetworking::Socket client = GNetworking::SocketAccept(server);
			std::cout << "Accepted connection..." << std::endl;

			std::cout << "Waiting for client to send data." << std::endl;
			while (!GNetworking::SocketPoll(client, GNetworking::PollEvent::PollIn)) std::this_thread::sleep_for(std::chrono::milliseconds(100));

			std::cout << "Recieving data." << std::endl;
			while (GNetworking::SocketPoll(client, GNetworking::PollEvent::PollIn)) {
				output = GNetworking::SocketRecv(client, buffer.data(), buffer.size(), GNetworking::SocketFlag::None);

				read.insert(read.end(), buffer.begin(), buffer.begin() + output);
			}

			std::cout << read.data() << std::endl;

			std::string send = "Hello TCP!";

			GNetworking::SocketSend(client, send.c_str(), send.length(), GNetworking::SocketFlag::None);
			std::cout << "Sent data..." << std::endl;
			GNetworking::SocketShutdown(client, GNetworking::SocketShutdownFlags::RDWR);
			std::cout << "Shutdown socket..." << std::endl;

			// Output should be 0 indicating shutdown with PIN packet
			output = GNetworking::SocketRecv(client, buffer.data(), buffer.size(), GNetworking::SocketFlag::None);
			std::cout << output << std::endl;

			GNetworking::SocketClose(client);
			std::cout << "Closed socket..." << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	GNetworking::SocketShutdown(server, GNetworking::SocketShutdownFlags::RDWR);
	GNetworking::SocketClose(server);
	GNetworking::SocketCleanup();
}
