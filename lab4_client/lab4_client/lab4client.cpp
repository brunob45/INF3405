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

	return leSocket.connect(host.c_str(), port.c_str());
}

bool connecterAuCompte() {
	string username = console::prompt("Saisir le nom d'utilisateur : ");
	string password = console::prompt("Saisir le mot de passe : ");

	leSocket.send("connexion:" + username + "," + password);
	return leSocket.read(1) == "A";
}

int __cdecl main(int argc, char **argv) {
	bool isConnected = false;

	leSocket = mySocket();
	while (!connecterAuServeur()) {
		console::writeLine(" Entree invalide. Veuillez reessayer.");
	}

	while (!connecterAuCompte()) {
		console::writeLine(" Impossible de se connecter. Veuillez reessayer");
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
		cout << msg;
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
	}
}

bool verifyIP(string ip) {
	regex expression("^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$.^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$.^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$.^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$.^([01][0-9][0-9]|2[0-4][0-9]|25[0-5])$");
	return regex_match(ip, expression);
}

bool verifyPort(string port) {
	if (atoi(port.c_str()) > 5000 && atoi(port.c_str()) < 5050)
		return true;
	else
		return false;
}