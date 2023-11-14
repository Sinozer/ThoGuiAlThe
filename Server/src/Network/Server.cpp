#include "Server.h"

Server::Server() : m_Port{"6969"}, m_ServerSocket(INVALID_SOCKET)
{
	WSADATA wsaData;
	if (int r = WSAStartup(MAKEWORD(2, 2), &wsaData); r != 0)
	{
		// Gestion de l'erreur
		LOG("WSAStartup failed with error: " << r);
		throw std::exception("WSAStartup failed");
	}
	else 
		LOG("WSAStartup success. Status: " << wsaData.szSystemStatus);
}

Server::~Server()
{
	CloseServer();

	WSACleanup();
}

void Server::StartServer()
{
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

void Server::AcceptNewClient()
{
	SOCKET clientSocket = accept(m_ServerSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
        throw std::exception("accept failed");
    }
	else
		LOG("accept success");

    m_ClientSockets.push_back(clientSocket);
}

void Server::CloseClient(SOCKET clientSocket)
{
	closesocket(clientSocket);
	// Retirez le client de la liste des clients

	for (int i = 0; i < m_ClientSockets.size(); i++)
	{
		if (m_ClientSockets[i] == clientSocket)
		{
			m_ClientSockets.erase(m_ClientSockets.begin() + i);
			break;
		}
	}
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
