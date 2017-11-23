#pragma once

#include <winsock2.h>
#include <string>

class mySocket
{
private:
	WSADATA wsaData;
	SOCKET leSocket;// = INVALID_SOCKET;

public:
	mySocket();
	~mySocket();
	bool connect(const char* host, const char* port) const;
	bool connect(const std::string host, const std::string port) const
	{
		return connect(host.c_str(), port.c_str());
	}
	bool send(const char* message);
	bool send(const std::string message)
	{
		return send(message.c_str());
	}
	std::string read(int taille);
};
