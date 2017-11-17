#undef UNICODE
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <iostream>;
#include <string>;
#include <regex>
#include "socket.h"

// Link avec ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

void startupChat();
void serverListener();
void clientListener();
bool verifyIP(std::string);
bool verifyPort(std::string);

mySocket leSocket;

int __cdecl main(int argc, char **argv)
{
	bool isConnected = false;

	while (!isConnected) {
		WSADATA wsaData;

		char username[80];
		char passwd[80];
		char motRecu[1];
		int iResult;

		// On va creer le socket pour communiquer avec le serveur
		leSocket = mySocket();
		//--------------------------------------------
		// On va chercher l'adresse du serveur en utilisant la fonction getaddrinfo.
		
		std::string host = "";
		std::string port = "";

		std::cout << "\nEntrez l'adresse ip du serveur : ";
		std::cin >> host;
		std::cout << "\nEntrez le port du serveur : ";
		std::cin >> port;


		if (verifyIP(host) && verifyPort(port)) {
			printf("format de l'adresse ip ou port incorrect");
			WSACleanup();
			return 1;
		}

		
		printf("Tentative de connexion au serveur %s avec le port %s\n\n", host, port);

		// On va se connecter au serveur en utilisant l'adresse qui se trouve dans
		// la variable result.
		while (!leSocket.connect(host.c_str(), port.c_str()))
		{
			/*console.writeline("Veuillez réessayer.");
			console.write("Nom de l'hote (132.207.29.XXX) : ");
			host = console.read();
			console.write("Numero de port (5000-5050) : ");
			port = console.read();*/
			std::cout << "\nEntrez l'adresse ip du serveur : ";
			std::cin >> host;
			std::cout << "\nEntrez le port du serveur : ";
			std::cin >> port;
		}

		printf("Connecte au serveur %s:%s\n\n", host, port);

		//----------------------------
		// Demander à l'usager un mot a envoyer au serveur
		printf("Saisir le nom d'utilisateur");
		std::string strUser = gets_s(username);
		printf("Saisir le mot de passe");
		std::string strPasswd = gets_s(passwd);

		//-----------------------------
		// Envoyer le mot au serveur
		leSocket.send((strUser + " " + strPasswd).c_str());

		//------------------------------
		// Maintenant, on va recevoir l' information envoyée par le serveur
		std::string rec = leSocket.read(1);
		if (rec == "a")
			isConnected = true;
	}

	startupChat();
	while (1);
	// cleanup

	WSACleanup();

	printf("Appuyez une touche pour finir\n");
	getchar();
	return 0;
}

void startupChat() {
	std::thread server(serverListener);
	std::thread client(clientListener);
}

void serverListener() {
	while (1) {
		std::string msg = leSocket.read(200);
		std::cout << msg;
	}
}

void clientListener() {
	while (1) {
		char motEnvoye[200];
		std::cin >> motEnvoye;
		std::cout << motEnvoye;
	}
}

bool verifyIP(std::string ip) {
	std::regex expression("^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$.^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$.^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$.^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$.^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$");
	return std::regex_match(ip, expression);
}

bool verifyPort(std::string port) {
	if (atoi(port.c_str()) > 5000 && atoi(port.c_str()) < 5050)
		return true;
	else
		return false;
}