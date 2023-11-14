#include "Network/Server.h"

int main()
{
	Server* server = new Server();

	server->StartServer();

	server->AcceptNewClient();

	delete server;
	return 0;
}
