#define _WINSOCK_DEPRECATED_NO_WARNINGS
#undef UNICODE

#include <ws2tcpip.h>
#include <winsock2.h>
#include <iostream>
#include <algorithm>
#include <strstream>
#include <locale>
#include <regex>

#include "socket_server.h"
#include "console.h"

using namespace std;


// link with Ws2_32.lib
#pragma comment( lib, "ws2_32.lib" )

mySocket_server leSocket;
bool continuer = true;


bool verifyIP(string ip) {
	regex expression("\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
	return regex_match(ip, expression);
}

bool verifyPort(string port) {
	return (atoi(port.c_str()) >= 5000 && atoi(port.c_str()) <= 5050);
}

bool connecterAuServeur() {
	string host = console::prompt("Entrez l'adresse ip du serveur : ");
	if (!verifyIP(host)) {
		return false;
	}

	string port = console::prompt("Entrez le port du serveur : ");
	if (!verifyPort(port)) {
		return false;
	}

	return leSocket.setup(host, port);
}

int main(void)
{
	string host, port;

	while (!connecterAuServeur()) {
		console::writeLine(" Entree invalide. Veuillez reessayer.");
	}

	while (true);
	return 0;
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
