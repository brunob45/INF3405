#pragma once

#include <winsock2.h>
#include <vector>
#include <string>

class mySocket
{
private:
	WSADATA wsaData;
	SOCKET leSocket;// = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char motEnvoye[10];
	char motRecu[10];
	int iResult;
	std::vector<std::string> inputbuf;
	bool inputbuf_dirty;

public:
	mySocket();
	~mySocket();
	bool connect(const char* host, const char* port);
	bool send(const char* message);
private:
	void receiveT();
};
