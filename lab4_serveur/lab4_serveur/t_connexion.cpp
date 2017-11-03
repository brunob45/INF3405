#include "t_connexion.h"
#include "common.h"
#include <thread>
#include <iostream>;

using namespace std;

void TConnexion::run() {
	std::thread t(TConnexion::threadFct);
}

void TConnexion::threadFct() {
	while (1) {
		sockaddr_in sinRemote;
		int nAddrSize = sizeof(sinRemote);
		// Create a SOCKET for accepting incoming requests.
		// Accept the connection.
		SOCKET sd = accept(ServerSocket, (sockaddr*)&sinRemote, &nAddrSize);
		if (sd != INVALID_SOCKET) {
			char readBuffer[32] = { 0 };
			recv(sd, readBuffer, 32, 0);
			//

		}
		else {
			// Do nothing.
		}
	}
}