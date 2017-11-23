#undef UNICODE

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include "socket.h"

mySocket::mySocket()
{
	bool valide = true;
	//--------------------------------------------
	// InitialisATION de Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("Erreur de WSAStartup: %d\n", iResult);
		valide = false;
	}
	if (!valide)
	{
		printf("Appuyez une touche pour finir\n");
		getchar();
		exit(1);
	}
}

mySocket::~mySocket()
{
	closesocket(leSocket);
	WSACleanup();
}

bool mySocket::connect(const char* host, const char* port)
{
	struct addrinfo* result = NULL;
	struct addrinfo	hints;

	// On va creer le socket pour communiquer avec le serveur
	leSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (leSocket == INVALID_SOCKET) {
		printf("Erreur de socket(): %ld\n\n", WSAGetLastError());
		return false;
	}

	//--------------------------------------------
	// On va chercher l'adresse du serveur en utilisant la fonction getaddrinfo.
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;        // Famille d'adresses
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  // Protocole utilisé par le serveur

	// getaddrinfo obtient l'adresse IP du host donné
	int iResult = getaddrinfo(host, port, &hints, &result);
	if (iResult != 0) {
		printf("Erreur de getaddrinfo: %d\n", iResult);
		return false;
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
		return false;
	}

	sockaddr_in *adresse;
	adresse = (struct sockaddr_in *) result->ai_addr;
	//----------------------------------------------------
	printf("Adresse trouvee pour le serveur %s : %s\n\n", host, inet_ntoa(adresse->sin_addr));
	printf("Tentative de connexion au serveur %s avec le port %s\n\n", inet_ntoa(adresse->sin_addr), port);

	// On va se connecter au serveur en utilisant l'adresse qui se trouve dans
	// la variable result.
	iResult = ::connect(leSocket, result->ai_addr, (int)(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		printf("Impossible de se connecter au serveur %s sur le port %s\n\n", inet_ntoa(adresse->sin_addr), port);
		freeaddrinfo(result);
		return false;
	}

	printf("Connecte au serveur %s:%s\n\n", host, port);
	freeaddrinfo(result);
	return true;
}

bool mySocket::send(const char* message)
{
	//-----------------------------
	// Envoyer le mot au serveur
	char buf[200] = { ' ' };
	memcpy(buf, message, strlen(message));

	int iResult = ::send(leSocket, buf, 200, 0);
	if (iResult == SOCKET_ERROR) {
		printf("Erreur du send: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}
std::string mySocket::read(int taille)
{
	char* buf = new char[taille];
	int iResult = recv(leSocket, buf, taille, 0);
	std::string ret = buf;
	delete[] buf;
	return ret;
}
