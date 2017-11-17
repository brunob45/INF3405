#undef UNICODE

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <iostream>;
#include <string>;

// Link avec ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

void startupChat();
void serverListener();
void clientListener();
bool verifyIP();
bool verifyPort();

SOCKET leSocket;

int __cdecl main(int argc, char **argv)
{
	bool isConnected = false;

	while (!isConnected) {
		WSADATA wsaData;

		struct addrinfo *result = NULL,
						*ptr = NULL,
						hints;
		char username[80];
		char passwd[80];
		char motRecu[1];
		int iResult;

		//--------------------------------------------
		// InitialisATION de Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("Erreur de WSAStartup: %d\n", iResult);
			return 1;
		}
		// On va creer le socket pour communiquer avec le serveur
		leSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (leSocket == INVALID_SOCKET) {
			printf("Erreur de socket(): %ld\n\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			printf("Appuyez une touche pour finir\n");
			getchar();
			return 1;
		}
		//--------------------------------------------
		// On va chercher l'adresse du serveur en utilisant la fonction getaddrinfo.
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;        // Famille d'adresses
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;  // Protocole utilisé par le serveur

										  // On indique le nom et le port du serveur auquel on veut se connecter
										  //char *host = "L4708-XX";
										  //char *host = "L4708-XX.lerb.polymtl.ca";
										  //char *host = "add_IP locale";
		
		// Demander l'adresse ip et le port du serveur au client
		std::string hostStr = "";
		std::string portStr = "";

		std::cout << "Entrez l'adresse ip du serveur : ";
		std::cin >> hostStr;
		std::cout << "Entrez le port du serveur : ";
		std::cin >> portStr;

		const char* host = hostStr.c_str();
		const char* port = portStr.c_str();

		if (verifyIP(host) && verifyPort(port)) {
			printf("format de l'adresse ip ou port incorrect");
			WSACleanup();
			return 1;
		}

		// getaddrinfo obtient l'adresse IP du host donné
		iResult = getaddrinfo(host, port, &hints, &result);
		if (iResult != 0) {
			printf("Erreur de getaddrinfo: %d\n", iResult);
			WSACleanup();
			return 1;
		}
		//---------------------------------------------------------------------		
		//On parcours les adresses retournees jusqu'a trouver la premiere adresse IPV4
		while ((result != NULL) && (result->ai_family != AF_INET))
			result = result->ai_next;

		//	if ((result != NULL) &&(result->ai_family==AF_INET)) result = result->ai_next;  

		//-----------------------------------------
		if (((result == NULL) || (result->ai_family != AF_INET))) {
			freeaddrinfo(result);
			printf("Impossible de recuperer la bonne adresse\n\n");
			WSACleanup();
			printf("Appuyez une touche pour finir\n");
			getchar();
			return 1;
		}

		sockaddr_in *adresse;
		adresse = (struct sockaddr_in *) result->ai_addr;
		//----------------------------------------------------
		printf("Adresse trouvee pour le serveur %s : %s\n\n", host, inet_ntoa(adresse->sin_addr));
		printf("Tentative de connexion au serveur %s avec le port %s\n\n", inet_ntoa(adresse->sin_addr), port);

		// On va se connecter au serveur en utilisant l'adresse qui se trouve dans
		// la variable result.
		iResult = connect(leSocket, result->ai_addr, (int)(result->ai_addrlen));
		if (iResult == SOCKET_ERROR) {
			printf("Impossible de se connecter au serveur %s sur le port %s\n\n", inet_ntoa(adresse->sin_addr), port);
			freeaddrinfo(result);
			WSACleanup();
			printf("Appuyez une touche pour finir\n");
			getchar();
			return 1;
		}

		printf("Connecte au serveur %s:%s\n\n", host, port);
		freeaddrinfo(result);

		//----------------------------
		// Demander à l'usager un mot a envoyer au serveur
		printf("Saisir le nom d'utilisateur");
		std::string strUser = gets_s(username);
		printf("Saisir le mot de passe");
		std::string strPasswd = gets_s(passwd);

		//-----------------------------
		// Envoyer le mot au serveur
		iResult = send(leSocket, (strUser + " " + strPasswd).c_str(), 161, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Erreur du send: %d\n", WSAGetLastError());
			closesocket(leSocket);
			WSACleanup();
			printf("Appuyez une touche pour finir\n");
			getchar();

			return 1;
		}

		//------------------------------
		// Maintenant, on va recevoir l' information envoyée par le serveur
		iResult = recv(leSocket, motRecu, 1, 0);
		if (iResult == 1) {
			if (motRecu[0] == 'a')
				isConnected = true;
		}
	}

	startupChat();
	while (1);
	// cleanup

	closesocket(leSocket);
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
		char motRecu[200];
		int result = recv(leSocket, motRecu, 200, 0);
		if (result > 0) {
			std::cout << motRecu;
		}
	}
}

void clientListener() {
	while (1) {
		char motEnvoye[200];
		std::cin >> motEnvoye;
		std::cout << motEnvoye;
	}
}

bool verifyIP(const char* ip) {
	return true;
}

bool verifyPort(const char* port) {
	if (atoi(port) > 5000 && atoi(port) < 5050)
		return true;
	else
		return false;
}