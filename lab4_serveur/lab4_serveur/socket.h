#pragma once

#include <WinSock2.h>
#include <thread>
#include <vector>

class mySocket_server
{
private:
	std::thread connexions_t;
	WSADATA wsaData;
public:
	mySocket_server();
	~mySocket_server();
	bool setup(int port);
	//void connexions();
private:
};