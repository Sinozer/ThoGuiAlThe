#include <Windows.h>
#include <stdio.h>
#include <vector>
#include "Network/Server.h"

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable run-time memory check for debug builds.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Server::GetInstance().StartServer();
	Server::GetInstance().ProcessMessages();


	return 0;
}
