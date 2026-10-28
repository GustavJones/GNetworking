#pragma once
#include <cstddef>
#include <string>
#include <cstdint>

#if defined _WIN32
#include <WinSock2.h>
#elif defined __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#endif // _WIN32

namespace GNetworking {

struct SocketSettings {
	virtual const void *Settings() const = 0;
	virtual const size_t SizeOfSettings() const = 0;

	virtual ~SocketSettings() = default;
};

enum class SocketLevel : int {
	Socket = SOL_SOCKET,
	TCP = IPPROTO_TCP
};

enum class SocketOption : int {
	Linger = SO_LINGER,
	ReuseAddr = SO_REUSEADDR,
	ReusePort = SO_REUSEPORT
};

enum class SocketAddressFamily : int {
	INET = AF_INET,
	INET6 = AF_INET6,
};

enum class SocketType : int {
	Stream = SOCK_STREAM,
	DGRAM = SOCK_DGRAM
};

enum class SocketFlag : int {
	None = 0,
	Peek = MSG_PEEK,
};

enum class SocketProtocol : int {
	Default = 0,
	IP = IPPROTO_IP,
	TCP = IPPROTO_TCP
};

enum class PollEvent : short {
	PollIn = POLLIN,
	PollOut = POLLOUT,
	PollHup = POLLHUP
};

#if defined _WIN32
enum class SocketShutdownFlags : int {
	RD = SD_RECIEVE,
	WR = SD_SEND,
	RDWR = SD_BOTH,
};

enum class Socket : int {
	Invalid = INVALID_SOCKET,
};

#elif defined __linux__
enum class SocketShutdownFlags : int {
	RD = SHUT_RD,
	WR = SHUT_WR,
	RDWR = SHUT_RDWR,
};

enum class Socket : int {
	Invalid = -1,
};

struct LingerSettings final : private linger, public SocketSettings {
	LingerSettings() {
		this->l_onoff = 0;
		this->l_linger = 0;
	}

	const void *Settings() const override {
		return dynamic_cast<const linger *>(this);
	}

	const size_t SizeOfSettings() const override {
		return sizeof(linger);
	}

	void SetStatus(int _status) { this->l_onoff = _status; }
	void SetSeconds(int _seconds) { this->l_linger = _seconds; }

	int GetStatus() const { return this->l_onoff; }
	int GetSeconds() const { return this->l_linger; }
};

struct ReuseAddrSettings final : public SocketSettings {
private:
	int m_enabled = 0;

public:
	const void *Settings() const override{
		return &m_enabled;
	}

	const size_t SizeOfSettings() const override {
		return sizeof(int);
	}

	void SetEnabled(int _enabled) { m_enabled = _enabled; }
	int GetEnabled() const { return m_enabled; }
};
#endif

class SocketFlagsBitMask {
private:
	int m_value = 0;
public:
	SocketFlagsBitMask() = default;
	SocketFlagsBitMask(const SocketFlag _flag) { m_value = (int)_flag; };

	SocketFlagsBitMask operator&(const SocketFlagsBitMask _event) const { return (SocketFlag)(_event.m_value & m_value); }
	SocketFlagsBitMask operator|(const SocketFlagsBitMask _event) const { return (SocketFlag)(_event.m_value | m_value); }
	SocketFlagsBitMask operator^(const SocketFlagsBitMask _event) const { return (SocketFlag)(_event.m_value ^ m_value); }
	SocketFlagsBitMask operator~() const { return (SocketFlag)(~m_value ); }

	SocketFlagsBitMask& operator&=(const SocketFlagsBitMask _event) {
		m_value &= _event.m_value;
		return *this;
	}

	SocketFlagsBitMask& operator|=(const SocketFlagsBitMask _event) {
		m_value |= _event.m_value;
		return *this;
	}

	SocketFlagsBitMask& operator^=(const SocketFlagsBitMask _event) {
		m_value ^= _event.m_value;
		return *this;
	}

	operator int() const { return m_value; }
};

inline SocketFlagsBitMask operator&(const SocketFlag _event1, const SocketFlag _event2) {
	return ((SocketFlagsBitMask)_event1 & (SocketFlagsBitMask)_event2);
}

inline SocketFlagsBitMask operator|(const SocketFlag _event1, const SocketFlag _event2) {
	return ((SocketFlagsBitMask)_event1 | (SocketFlagsBitMask)_event2);
}

inline SocketFlagsBitMask operator^(const SocketFlag _event1, const SocketFlag _event2) {
	return ((SocketFlagsBitMask)_event1 ^ (SocketFlagsBitMask)_event2);
}

inline SocketFlagsBitMask operator~(const SocketFlag _event1) {
	return ~((SocketFlagsBitMask)_event1);
}

class PollEventsBitMask {
private:
	short m_value = 0;
public:
	PollEventsBitMask() = default;
	PollEventsBitMask(const PollEvent _event) { m_value = (short)_event; };

	PollEventsBitMask operator&(const PollEventsBitMask _event) const { return (PollEvent)(_event.m_value & m_value); }
	PollEventsBitMask operator|(const PollEventsBitMask _event) const { return (PollEvent)(_event.m_value | m_value); }
	PollEventsBitMask operator^(const PollEventsBitMask _event) const { return (PollEvent)(_event.m_value ^ m_value); }
	PollEventsBitMask operator~() const { return (PollEvent)(~m_value ); }

	PollEventsBitMask& operator&=(const PollEventsBitMask _event) {
		m_value &= _event.m_value;
		return *this;
	}

	PollEventsBitMask& operator|=(const PollEventsBitMask _event) {
		m_value |= _event.m_value;
		return *this;
	}

	PollEventsBitMask& operator^=(const PollEventsBitMask _event) {
		m_value ^= _event.m_value;
		return *this;
	}

	operator short() const { return m_value; }
};

inline PollEventsBitMask operator&(const PollEvent _event1, const PollEvent _event2) {
	return ((PollEventsBitMask)_event1 & (PollEventsBitMask)_event2);
}

inline PollEventsBitMask operator|(const PollEvent _event1, const PollEvent _event2) {
	return ((PollEventsBitMask)_event1 | (PollEventsBitMask)_event2);
}

inline PollEventsBitMask operator^(const PollEvent _event1, const PollEvent _event2) {
	return ((PollEventsBitMask)_event1 ^ (PollEventsBitMask)_event2);
}

inline PollEventsBitMask operator~(const PollEvent _event1) {
	return ~((PollEventsBitMask)_event1);
}

int SocketSetup();
int SocketCleanup();

[[nodiscard]]
Socket SocketAccept(const Socket &_socket);
int SocketBind(const Socket &_socket, const std::string &_address, const uint16_t _port);
int SocketListen(const Socket &_socket);
int SocketConnect(const Socket &_socket, const std::string &_address, const uint16_t _port);

[[nodiscard]]
Socket SocketCreate(const SocketAddressFamily _family, const SocketType _type, const SocketProtocol _proto);
int SocketShutdown(const Socket &_socket, const SocketShutdownFlags _flags);
int SocketClose(const Socket &_socket);

int SocketRecv(const Socket &_socket, char *_buffer, const size_t _bufferLen, const SocketFlagsBitMask _flags);
int SocketPeek(const Socket &_socket, char *_buffer, const size_t _bufferLen, const SocketFlagsBitMask _flags);
int SocketSend(const Socket &_socket, const char *_buffer, const size_t _bufferLen, const SocketFlagsBitMask _flags);

[[nodiscard]]
bool SocketPoll(const Socket &_socket, const PollEventsBitMask _events);

[[nodiscard]]
size_t SocketPollSize(const Socket &_socket);

int SocketSetOption(const Socket &_socket, const SocketLevel _level, const SocketOption _option, const SocketSettings &_value);
} // namespace Wepp
