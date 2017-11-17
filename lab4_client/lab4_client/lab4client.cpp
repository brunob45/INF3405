// lab4client.cpp : Définit le point d'entrée pour l'application console.
//

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>


using namespace std;

bool _continue = false;
void killThread()
{
	do
	{
		_continue = (GetAsyncKeyState(VK_ESCAPE) == 0);
	} while (_continue);
	exit(0);
}

vector<string> split(string s, char c)
{
	vector<string> ret;
	int index, offset;
	for (index = 0, offset = 0; index < s.length(); index++)
	{
		if (s.at(index) == c)
		{
			if (offset < index)
			{
				ret.push_back(s.substr(offset, index - offset));
			}
			offset = index+1;
		}
	}
	if (offset < index)
	{
		ret.push_back(s.substr(offset, index - offset));
	}
	return ret;
}

string getIp()
{
	cout << "Entrer l'adresse IP du serveur : ";
	string ip = "";
	bool isValid = true;
	do
	{
		isValid = true;
		getline(cin, ip);

		for (int i = 0; i < ip.length(); i++)
		{
			if (!isdigit(ip[i]) && ip[i] != '.')
			{
				isValid = false;
				break;
			}
		}

		int numberCount = 0;
		vector<string> t = split(ip, '.');
		for (int i = 0; i < t.size(); i++)
		{
			int temp = atoi(t[i].c_str());
			if (temp >= 0 && temp < 256)
			{
				numberCount++;
			}
		}
		isValid &= (numberCount == 4);

		if (!isValid)
		{
			cout << "Erreur dans l'entree. Veuillez reessayer : ";
		}
	} while (!isValid);
	return ip;
}

string getPort()
{
	cout << "Entrer le port associe au serveur : ";
	string port = "";
	int nPort = 0;
	bool isValid = true;
	do
	{
		getline(cin, port);
		nPort = atoi(port.c_str());
		isValid = (nPort >= 5000) && (nPort <= 5050);
		if (!isValid)
		{
			cout << "Erreur dans l'entree. Veuillez reessayer : ";
		}
	} while (!isValid);
	return port;
}

SOCKET connect(string ip, string port)
{
	SOCKET leSocket = { 0 };
	WSADATA wsaData;
	//--------------------------------------------
	// InitialisATION de Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("Erreur de WSAStartup: %d\n", iResult);
		_continue = false;
		return leSocket;
	}
	// On va creer le socket pour communiquer avec le serveur
	leSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (leSocket == INVALID_SOCKET) {
		printf("Erreur de socket(): %ld\n\n", WSAGetLastError());
		WSACleanup();
		printf("Appuyez une touche pour finir\n");
		getchar();
		_continue = false;
		return leSocket;
	}


}

int main()
{
	thread killT(killThread);

	//ask ip
	string ip = getIp();
	//ask port
	string port = getPort();
	//connect to server
	SOCKET leSocket = connect(ip, port);

	//start reception thread


	while (_continue)
	{
		//read message
		//send message to server
	}

	killT.join();
    return 0;
}

