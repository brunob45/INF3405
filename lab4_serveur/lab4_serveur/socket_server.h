#pragma once

#include <WinSock2.h>
#include <vector>
#include <thread>
#include <string>

class mySocket_server
{
private:
	std::thread connexions_t;
	WSADATA wsaData;
	SOCKET ServerSocket;
public:
	mySocket_server();
	~mySocket_server();
	bool setup(std::string host, std::string port);
	//void connexions();
	static const char* WSAGetLastErrorMessage(const char* pcMessagePrefix, int nErrorID = 0);
	
};