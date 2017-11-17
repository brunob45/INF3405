#undef UNICODE

#include <winsock2.h>
#include <iostream>
#include <algorithm>
#include <strstream>
#include <locale>

#include "socket.h"

using namespace std;

// link with Ws2_32.lib
#pragma comment( lib, "ws2_32.lib" )

// External functions
//extern DWORD WINAPI EchoHandler(void* sd_) ;
//extern void DoSomething( char *src, char *dest );




int main(void) 
{
	mySocket_server server;
	server.setup(5001);
	while (true);
}

/*
//// EchoHandler ///////////////////////////////////////////////////////
// Handles the incoming data by reflecting it back to the sender.

DWORD WINAPI EchoHandler(void* sd_) 
{
	SOCKET sd = (SOCKET)sd_;

	// Read Data from client
	char readBuffer[10], outBuffer[10];
	int readBytes;

	readBytes = recv(sd, readBuffer, 7, 0);
	if (readBytes > 0) {
        cout << "Received " << readBytes << " bytes from client." << endl;
		cout << "Received " << readBuffer << " from client." << endl;
		DoSomething(readBuffer, outBuffer);
		send(sd, outBuffer, 7, 0);
	}
	else if (readBytes == SOCKET_ERROR) {
		cout << WSAGetLastErrorMessage("Echec de la reception !") << endl;
	}
	closesocket(sd);

	return 0;
}
// Do Something with the information
void DoSomething( char *src, char *dest )
{
	auto index = 0;
	for (auto i = 6; i >= 0; i--)
	{
		dest[index++] = (i % 2 != 0) ? src[i] : toupper(src[i]);
	}
}*/
