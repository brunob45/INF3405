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
