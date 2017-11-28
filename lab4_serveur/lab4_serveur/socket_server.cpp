#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ws2tcpip.h>
#include <winsock2.h>
#include <iostream>
#include <algorithm>
#include <strstream>
#include <locale>
#include <vector>
#include <thread>
#include "socket_server.h"
#include "db.h"

using namespace std;

void connexions(SOCKET server);
void onReceive(const SOCKET& sd);
vector<SOCKET> clients;
vector<thread> threads;
db users;
db chat;


// List of Winsock error constants mapped to an interpretation string.
// Note that this list must remain sorted by the error constants'
// values, because we do a binary search on the list when looking up
// items.
static struct ErrorEntry {
	int nID;
	const char* pcMessage;

	ErrorEntry(int id, const char* pc = 0) :
		nID(id),
		pcMessage(pc)
	{
	}

	bool operator<(const ErrorEntry& rhs) const
	{
		return nID < rhs.nID;
	}
} gaErrorList[] = {
	ErrorEntry(0,                  "No error"),
	ErrorEntry(WSAEINTR,           "Interrupted system call"),
	ErrorEntry(WSAEBADF,           "Bad file number"),
	ErrorEntry(WSAEACCES,          "Permission denied"),
	ErrorEntry(WSAEFAULT,          "Bad address"),
	ErrorEntry(WSAEINVAL,          "Invalid argument"),
	ErrorEntry(WSAEMFILE,          "Too many open sockets"),
	ErrorEntry(WSAEWOULDBLOCK,     "Operation would block"),
	ErrorEntry(WSAEINPROGRESS,     "Operation now in progress"),
	ErrorEntry(WSAEALREADY,        "Operation already in progress"),
	ErrorEntry(WSAENOTSOCK,        "Socket operation on non-socket"),
	ErrorEntry(WSAEDESTADDRREQ,    "Destination address required"),
	ErrorEntry(WSAEMSGSIZE,        "Message too long"),
	ErrorEntry(WSAEPROTOTYPE,      "Protocol wrong type for socket"),
	ErrorEntry(WSAENOPROTOOPT,     "Bad protocol option"),
	ErrorEntry(WSAEPROTONOSUPPORT, "Protocol not supported"),
	ErrorEntry(WSAESOCKTNOSUPPORT, "Socket type not supported"),
	ErrorEntry(WSAEOPNOTSUPP,      "Operation not supported on socket"),
	ErrorEntry(WSAEPFNOSUPPORT,    "Protocol family not supported"),
	ErrorEntry(WSAEAFNOSUPPORT,    "Address family not supported"),
	ErrorEntry(WSAEADDRINUSE,      "Address already in use"),
	ErrorEntry(WSAEADDRNOTAVAIL,   "Can't assign requested address"),
	ErrorEntry(WSAENETDOWN,        "Network is down"),
	ErrorEntry(WSAENETUNREACH,     "Network is unreachable"),
	ErrorEntry(WSAENETRESET,       "Net connection reset"),
	ErrorEntry(WSAECONNABORTED,    "Software caused connection abort"),
	ErrorEntry(WSAECONNRESET,      "Connection reset by peer"),
	ErrorEntry(WSAENOBUFS,         "No buffer space available"),
	ErrorEntry(WSAEISCONN,         "Socket is already connected"),
	ErrorEntry(WSAENOTCONN,        "Socket is not connected"),
	ErrorEntry(WSAESHUTDOWN,       "Can't send after socket shutdown"),
	ErrorEntry(WSAETOOMANYREFS,    "Too many references, can't splice"),
	ErrorEntry(WSAETIMEDOUT,       "Connection timed out"),
	ErrorEntry(WSAECONNREFUSED,    "Connection refused"),
	ErrorEntry(WSAELOOP,           "Too many levels of symbolic links"),
	ErrorEntry(WSAENAMETOOLONG,    "File name too long"),
	ErrorEntry(WSAEHOSTDOWN,       "Host is down"),
	ErrorEntry(WSAEHOSTUNREACH,    "No route to host"),
	ErrorEntry(WSAENOTEMPTY,       "Directory not empty"),
	ErrorEntry(WSAEPROCLIM,        "Too many processes"),
	ErrorEntry(WSAEUSERS,          "Too many users"),
	ErrorEntry(WSAEDQUOT,          "Disc quota exceeded"),
	ErrorEntry(WSAESTALE,          "Stale NFS file handle"),
	ErrorEntry(WSAEREMOTE,         "Too many levels of remote in path"),
	ErrorEntry(WSASYSNOTREADY,     "Network system is unavailable"),
	ErrorEntry(WSAVERNOTSUPPORTED, "Winsock version out of range"),
	ErrorEntry(WSANOTINITIALISED,  "WSAStartup not yet called"),
	ErrorEntry(WSAEDISCON,         "Graceful shutdown in progress"),
	ErrorEntry(WSAHOST_NOT_FOUND,  "Host not found"),
	ErrorEntry(WSANO_DATA,         "No host data of that type was found")
};
const int kNumMessages = sizeof(gaErrorList) / sizeof(ErrorEntry);

//// WSAGetLastErrorMessage ////////////////////////////////////////////
// A function similar in spirit to Unix's perror() that tacks a canned 
// interpretation of the value of WSAGetLastError() onto the end of a
// passed string, separated by a ": ".  Generally, you should implement
// smarter error handling than this, but for default cases and simple
// programs, this function is sufficient.
//
// This function returns a pointer to an internal static buffer, so you
// must copy the data from this function before you call it again.  It
// follows that this function is also not thread-safe.
const char* mySocket_server::WSAGetLastErrorMessage(const char* pcMessagePrefix, int nErrorID)
{
	// Build basic error string
	static char acErrorBuffer[256];
	ostrstream outs(acErrorBuffer, sizeof(acErrorBuffer));
	outs << pcMessagePrefix << ": ";

	// Tack appropriate canned message onto end of supplied message 
	// prefix. Note that we do a binary search here: gaErrorList must be
	// sorted by the error constant's value.
	ErrorEntry* pEnd = gaErrorList + kNumMessages;
	ErrorEntry Target(nErrorID ? nErrorID : WSAGetLastError());
	ErrorEntry* it = lower_bound(gaErrorList, pEnd, Target);
	if ((it != pEnd) && (it->nID == Target.nID)) {
		outs << it->pcMessage;
	}
	else {
		// Didn't find error in list, so make up a generic one
		outs << "unknown error";
	}
	outs << " (" << Target.nID << ")";

	// Finish error message off and return it.
	outs << ends;
	acErrorBuffer[sizeof(acErrorBuffer) - 1] = '\0';
	return acErrorBuffer;
}

mySocket_server::mySocket_server()
	: ServerSocket(INVALID_SOCKET)
{
	//----------------------
	// Initialize Winsock.
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		cerr << "Error at WSAStartup()\n" << endl;
		getchar();
		exit(1);
	}

	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		cerr << WSAGetLastErrorMessage("Error at socket()") << endl;
		WSACleanup();
		getchar();
		exit(1);
	}
	char option[] = "1";
	setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, option, sizeof(option));

	users.open("users.txt");
	chat.open("chat.txt");
}

mySocket_server::~mySocket_server()
{
	if (ServerSocket != INVALID_SOCKET) {
		closesocket(ServerSocket);
	}
	WSACleanup();
}

bool mySocket_server::setup(std::string host, std::string port)
{
	struct addrinfo* service = NULL;
	struct addrinfo	hints;

	//--------------------------------------------
	// On va chercher l'adresse du serveur en utilisant la fonction getaddrinfo.
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;        // Famille d'adresses
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  // Protocole utilisé par le serveur
	hints.ai_flags = AI_PASSIVE;

	// getaddrinfo obtient l'adresse IP du host donné
	int iResult = getaddrinfo(host.c_str(), port.c_str(), &hints, &service);
	if (iResult != 0) {
		printf("Erreur de getaddrinfo: %d\n", iResult);
		return false;
	}

	// Create a SOCKET for connecting to server
	ServerSocket = socket(service->ai_family, service->ai_socktype, service->ai_protocol);
	if (ServerSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(service);
		return false;
	}

	// Setup the TCP listening socket
	iResult = ::bind(ServerSocket, service->ai_addr, (int)service->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(service);
		return false;
	}

	printf("En attente des connections des clients sur le port %s...\n\n", port.c_str());
	freeaddrinfo(service);

	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ServerSocket, 30) == SOCKET_ERROR) {
		cerr << WSAGetLastErrorMessage("Error listening on socket.") << endl;
		return false;
	}

	connexions_t = thread(connexions, ServerSocket);
	return true;
}


void connexions(SOCKET server)
{
	while (true)
	{
		sockaddr_in sinRemote;
		int nAddrSize = sizeof(sinRemote);
		// Create a SOCKET for accepting incoming requests.
		// Accept the connection.
		SOCKET sd = accept(server, (sockaddr*)&sinRemote, &nAddrSize);
		if (sd != INVALID_SOCKET) {
			cout << "Connection acceptee De : " <<
				inet_ntoa(sinRemote.sin_addr) << ":" <<
				ntohs(sinRemote.sin_port) << "." <<
				endl;

			clients.push_back(sd);
			threads.push_back(thread(onReceive, sd));
		}
		else {
			cerr << mySocket_server::WSAGetLastErrorMessage("Echec d'une connection.") <<
				endl;
		}
	}
}

void onReceive(const SOCKET& sd)
{
	bool connecte = false;
	char rxBuf[200] = { 0 };
	int readBytes = 0;
	string username = "";

	// connexion
	while (!connecte)
	{
		username = "unknown";
		readBytes = recv(sd, rxBuf, 200, 0);
		if (readBytes > 0) {
			cout << "Received " << rxBuf << " from " << username << endl;

			int index = 0;
			for (index = 0; index < 200 && rxBuf[index] != '\0' && rxBuf[index] != ':'; index++);

			username = "";
			string pass = "";
			for (index++; index < 200 && rxBuf[index] != '\0' && rxBuf[index] != ','; index++)
			{
				username += rxBuf[index];
			}
			for (index++; index < 200 && rxBuf[index] != '\0' && rxBuf[index] != ','; index++)
			{
				pass += rxBuf[index];
			}

			//verifier les credentials

			char reponse = 'N';
			if (!users.contains(username))
			{
				reponse = 'R';
				send(sd, &reponse, 1, 0);
				readBytes = recv(sd, rxBuf, 200, 0);
				if (rxBuf[0] == 'Y' || rxBuf[0] == 'y' || rxBuf[0] == 'o' || rxBuf[0] == 'O')
					users.add(username, pass, true);
				
				reponse = 'Y';
				connecte = true;
			}

			if (users.fetch(username) == pass)
			{
				reponse = 'Y';
				connecte = true;
			}
			send(sd, &reponse, 1, 0);
			cout << username << " connecte." << endl;
		}
		else if (readBytes == SOCKET_ERROR) {
			cout << mySocket_server::WSAGetLastErrorMessage("Echec de la reception !") << endl;
		}
	}

	while (recv(sd, rxBuf, 200, 0) > 0) {
		cout << "Received " << rxBuf << " from " << username << endl;
		for (int i = 0; i < clients.size(); i++) {
			if (clients[i] == sd) {
				continue;
			}
			send(clients[i], rxBuf, 200, 0);
		}
	}

	cout << "Connexion interrompue: " << username << endl;
}