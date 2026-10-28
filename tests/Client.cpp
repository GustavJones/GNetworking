#include "GNetworking/Socket.hpp"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, const char *argv[]) {
	std::vector<char> read;
	std::vector<char> buffer(4096);
	std::string message = "Hello from the other side?!";

	GNetworking::SocketSetup();

	GNetworking::Socket client = GNetworking::SocketCreate(GNetworking::SocketAddressFamily::INET, GNetworking::SocketType::Stream, (GNetworking::SocketProtocol)0);
	GNetworking::SocketConnect(client, "127.0.0.1", 8080);

	std::cout << "Sending data" << std::endl;
	GNetworking::SocketSend(client, message.c_str(), message.length(), GNetworking::SocketFlag::None);

	std::cout << "Waiting for data to read" << std::endl;
	while (!GNetworking::SocketPoll(client, GNetworking::PollEvent::PollIn)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	std::cout << "Reading data" << std::endl;
	while (GNetworking::SocketPoll(client, GNetworking::PollEvent::PollIn)) {
		int output = GNetworking::SocketRecv(client, buffer.data(), buffer.size(), GNetworking::SocketFlag::None);

		read.insert(read.end(), buffer.begin(), buffer.begin() + output);

		// PIN packet recieved after shutdown()
		if (output == 0) break;
	}

	std::cout << read.data() << std::endl;

	GNetworking::SocketShutdown(client, GNetworking::SocketShutdownFlags::RDWR);
	GNetworking::SocketClose(client);

	GNetworking::SocketCleanup();
}
