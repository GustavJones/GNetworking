# GNetworking

## Table of Content

1. [Description](#description) 
2. [Installation](#installation)
3. [Usage](#usage)

## Content

### Description
This is a simple library that contains an implementation of platform specific network functions in a cross-platform way.

### Installation

### Usage

Proper usage is detailed here:

Server:
```C++
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
```

Client:
```C++
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
```
