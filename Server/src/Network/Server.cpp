#include "Server.h"

#define MSG_SERVER (WM_USER + 1)
#define MSG_CLIENT (WM_USER + 2)

Server::Server() : m_hWnd(nullptr), m_Port{"6969"}, m_ServerSocket(INVALID_SOCKET)
{}

Server::~Server()
{
	CloseServer();

	WSACleanup();
}

void Server::StartServer()
{
	InitWindow();
	
	// Initialize Winsock
	WSADATA wsaData;
	if (int r = WSAStartup(MAKEWORD(2, 2), &wsaData); r != 0)
	{
		LOG("WSAStartup failed with error: " << r);
		throw std::exception("WSAStartup failed");
	}
	else
		LOG("WSAStartup success. Status: " << wsaData.szSystemStatus);

	addrinfo* result = nullptr;
	addrinfo hints{};

	// Resolve address and port
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE; // For wildcard IP address

	if (int r = getaddrinfo("localhost", m_Port, &hints, &result); r != 0)
	{
		LOG("getaddrinfo failed with error: " << r);
		WSACleanup();
		throw std::exception("getaddrinfo failed");
	}
	else
		LOG("getaddrinfo success");

	// Socket creation
	m_ServerSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_ServerSocket == INVALID_SOCKET)
	{
		LOG("socket failed with error: " << WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		throw std::exception("ServerSocket creation failed");
	}
	else
		LOG("socket success");

	// Bind socket to listen to TCP requests on the given address and port

	if (int r = bind(m_ServerSocket, result->ai_addr, (int)result->ai_addrlen); r == SOCKET_ERROR)
	{
		LOG("bind failed with error: " << WSAGetLastError());
		freeaddrinfo(result);
		closesocket(m_ServerSocket);
		WSACleanup();
		throw std::exception("bind failed");
	}
	else
		LOG("bind success");

	freeaddrinfo(result);

	if (int r = listen(m_ServerSocket, SOMAXCONN); r == SOCKET_ERROR)
	{
		LOG("listen failed with error: " << WSAGetLastError());
		closesocket(m_ServerSocket);
		WSACleanup();
		throw std::exception("listen failed");
	}
	else
		LOG("listen success");

	// Setup asynchronous socket for listening to new clients in the window message loop
	if (WSAAsyncSelect(m_ServerSocket, m_hWnd, MSG_SERVER, FD_ACCEPT) == SOCKET_ERROR)
	{
		LOG("WSAAsyncSelect failed with error: " << WSAGetLastError());
		closesocket(m_ServerSocket);
		WSACleanup();
		throw std::exception("WSAAsyncSelect failed");
	}
	else
		LOG("WSAAsyncSelect success");
}

void Server::ProcessMessages()
{
	MSG msg{};
	while (GetMessage(&msg, m_hWnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Server::CloseServer()
{
	if (m_ServerSocket != INVALID_SOCKET)
	{
		closesocket(m_ServerSocket);
		m_ServerSocket = INVALID_SOCKET;
	}
	// Fermez toutes les connexions avec les clients, si nécessaire

	for (SOCKET clientSocket : m_ClientSockets)
	{
		closesocket(clientSocket);
	}
}

void Server::AcceptNewClient(SOCKET& clientSocket)
{
	if (clientSocket == INVALID_SOCKET)
	{
		throw std::exception("accept failed");
	}
	else
		LOG("accept success");

	if (WSAAsyncSelect(clientSocket, m_hWnd, MSG_CLIENT, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		LOG("WSAAsyncSelect failed with error: " << WSAGetLastError());
		closesocket(clientSocket);
		return;
	}
	else
		LOG("WSAAsyncSelect success");

	m_ClientSockets.push_back(clientSocket);
	LOG("Number of clients: " << m_ClientSockets.size());
}

void Server::CloseClient(SOCKET clientSocket)
{
	if (int r = closesocket(clientSocket) != 0)
	{
		LOG("closesocket failed with error: " << r);
		return;
	}
    else
        LOG("closesocket success");
	// Retirez le client de la liste des clients

	for (int i = 0; i < m_ClientSockets.size(); i++)
	{
		if (m_ClientSockets[i] == clientSocket)
		{
			m_ClientSockets.erase(m_ClientSockets.begin() + i);
			break;
		}
	}
	LOG("Number of clients: " << m_ClientSockets.size());
}

// Implémentez des méthodes pour gérer la communication avec les clients ici

bool Server::SendToClient(SOCKET clientSocket, const char* data, int size)
{
	int bytesSent = send(clientSocket, data, size, 0);
	if (bytesSent == SOCKET_ERROR)
	{
		// Gestion de l'erreur

		std::cout << "Error sending data." << std::endl;

		return false;
	}

	return true;
}

bool Server::ReceiveFromClient(SOCKET clientSocket, char* data, int size)
{
	int bytesReceived = recv(clientSocket, data, size, 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		// Gestion de l'erreur
		return false;
	}

	return true;
}

bool Server::SendToAllClients(const char* data, int size)
{
	for (SOCKET clientSocket : m_ClientSockets)
	{
		std::cout << m_ClientSockets.size() << std::endl;
		if (!SendToClient(clientSocket, data, size))
		{
			return false;
		}
	}

	return true;
}

bool Server::ReceiveFromAllClients(char* data, int size)
{
	for (SOCKET clientSocket : m_ClientSockets)
	{
		if (!ReceiveFromClient(clientSocket, data, size))
		{
			return false;
		}
	}

	return true;
}

void Server::InitWindow()
{
	// Create an invisible window for message processing
	WNDCLASSEX wcex =
	{
		.cbSize = sizeof(WNDCLASSEX),
		.style = 0,
		.lpfnWndProc = WndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = GetModuleHandle(nullptr),
		.hIcon = nullptr,
		.hCursor = nullptr,
		.hbrBackground = nullptr,
		.lpszMenuName = nullptr,
		.lpszClassName = L"ServerWindow",
		.hIconSm = nullptr
	};
	RegisterClassEx(&wcex);

	m_hWnd = CreateWindowEx(0, L"ServerWindow", L"ServerWindow", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, GetModuleHandle(nullptr), nullptr);

	if (m_hWnd == nullptr)
	{
		LOG("CreateWindowEx failed with error: " << GetLastError());
		throw std::exception("CreateWindowEx failed");
	}
	else
		LOG("CreateWindowEx success");
}

LRESULT Server::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case MSG_SERVER:
	{
		if (WSAGETSELECTERROR(lParam))
		{
			LOG("FD_ACCEPT failed with error: " << WSAGetLastError());
			break;
		}

		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_ACCEPT:
			SOCKET clientSocket = accept(wParam, nullptr, nullptr);
			Server::GetInstance().AcceptNewClient(clientSocket);
			break;
		}

		return 0;
	}
	case MSG_CLIENT:
		if (WSAGETSELECTERROR(lParam))
		{
			LOG("FD_READ failed with error: " << WSAGetLastError());
			break;
		}

		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
		{
			char buffer[256];
			int bytesReceived = recv((SOCKET)wParam, buffer, sizeof(buffer), 0);
			if (bytesReceived == SOCKET_ERROR)
			{
				LOG("recv failed with error: " << WSAGetLastError());
				Server::GetInstance().CloseClient(wParam);
				break;
			}
			else if (bytesReceived == 0)
			{
				LOG("Connection closing...");
				Server::GetInstance().CloseClient(wParam);
				break;
			}
			else
			{
				LOG("Bytes received: " << bytesReceived);
				buffer[bytesReceived] = '\0';
				LOG("Message received: " << buffer);
			}

			break;
		}
		case FD_WRITE:
			break;
		case FD_CLOSE:
			Server::GetInstance().CloseClient(wParam);
            break;
		}
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // Call default message handler
}
