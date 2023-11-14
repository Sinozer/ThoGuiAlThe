#include "Network/Server.h"

int main()
{
	Server::GetInstance().StartServer();
	Server::GetInstance().ProcessMessages();

	return 0;
}
