//#undef UNICODE

#include <thread>
#include <string>
#include <regex>
#include "socket.h"
#include "console.h"

// Link avec ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

using namespace std;

mySocket leSocket;
bool continuer = true;

void serverListener();
void clientListener();
bool verifyIP(string);
bool verifyPort(string);

bool connecterAuServeur() {
	string host = console::prompt("Entrez l'adresse ip du serveur : ");
	if (!verifyIP(host)) {
		return false;
	}

	string port = console::prompt("Entrez le port du serveur : ");
	if (!verifyPort(port)) {
		return false;
	}

	return leSocket.connect(host, port);
}

bool connecterAuCompte() {
	string username = console::prompt("Saisir le nom d'utilisateur : ");
	string password = console::prompt("Saisir le mot de passe : ");
	leSocket.send("connexion:" + username + "," + password);
	string ret = leSocket.read(1);
	return ret[0] == 'Y';
}

int __cdecl main(int argc, char **argv) {
	bool isConnected = false;

	leSocket = mySocket();
	while (!connecterAuServeur()) {
		console::writeLine("\n Entree invalide. Veuillez reessayer.");
	}

	while (!connecterAuCompte()) {
		console::writeLine("\n Impossible de se connecter. Veuillez reessayer");
	}

	// partir les thread
	thread server(serverListener);
	thread client(clientListener);

	// attendre le signal de fin
	while (continuer);

	// terminer les thread
	server.join();
	client.join();

	console::write(" Appuyez une touche pour finir");
	console::readChar();
	return 0;
}

void serverListener() {
	while (continuer) {
		string msg = leSocket.read(200);
		cout << msg << endl;
	}
}

void clientListener() {
	while (continuer) {
		string message = console::readLine();
		if (message.length() > 200) {
			message = message.substr(0, 200);
		}
		if (message.length() == 0) {
			continuer = false;
		}
		leSocket.send(message);
	}
}

bool verifyIP(string ip) {
	regex expression("\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
	return regex_match(ip, expression);
}

bool verifyPort(string port) {
	if (atoi(port.c_str()) >= 5000 && atoi(port.c_str()) <= 5050)
		return true;
	else
		return false;
}