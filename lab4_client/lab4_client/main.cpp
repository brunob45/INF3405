
#include <iostream>
#include <string>
#include "socket.h"
#include "console.h"

using namespace std;

void GetCursorPosition(short& x, short& y)
{
	PCONSOLE_SCREEN_BUFFER_INFO info = NULL;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), info);
	x = info->dwCursorPosition.X;
	y = info->dwCursorPosition.Y;
}

void SetCursorPosition(const short& x, const short& y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
}

int GetConsoleWidth()
{
	PCONSOLE_SCREEN_BUFFER_INFO info = (PCONSOLE_SCREEN_BUFFER_INFO)malloc(sizeof(PCONSOLE_SCREEN_BUFFER_INFO));
	
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), info);
	return info->dwSize.X;
}

int main(int argc, char **argv)
{
	mySocket leSocket;
	string host = "132.207.29.XXX";
	string port = "5000";

	console.writeline("Connexion au serveur.");
	console.write("Nom de l'hote (132.207.29.XXX) : ");
	host = console.read();
	console.write("Numero de port (5000-5050) : ");
	port = console.read();

	while (!leSocket.connect(host.c_str(), port.c_str()))
	{
		console.writeline("Veuillez réessayer.");
		console.write("Nom de l'hote (132.207.29.XXX) : ");
		host = console.read();
		console.write("Numero de port (5000-5050) : ");
		port = console.read();
	}

	console.clear();

	bool _continue = true;
	do
	{
		SetCursorPosition(0, 0);
		console.write("                                                                                \r");
		string message = console.read();
		for (int i = 0; i < GetConsoleWidth()-message.length(); i++)
		{
			cout << " ";
		}
		cout << message << endl;
		leSocket.send(message.c_str());

	} while (_continue);
}