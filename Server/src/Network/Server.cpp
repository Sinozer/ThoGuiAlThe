#include "Server.h"

#include "Exceptions/TgatException.h"
#include "Player.h"

#define MSG_SERVER (WM_USER + 1)
#define MSG_CLIENT (WM_USER + 2)

Server::Server() : TgatNetworkHelper(), m_hWnd(nullptr), m_Port{ "6969" }
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
	m_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_Socket == INVALID_SOCKET)
	{
		LOG("socket failed with error: " << WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		throw std::exception("ServerSocket creation failed");
	}
	else
		LOG("socket success");

	// Bind socket to listen to TCP requests on the given address and port

	if (int r = bind(m_Socket, result->ai_addr, (int)result->ai_addrlen); r == SOCKET_ERROR)
	{
		LOG("bind failed with error: " << WSAGetLastError());
		freeaddrinfo(result);
		closesocket(m_Socket);
		WSACleanup();
		throw std::exception("bind failed");
	}
	else
		LOG("bind success");

	freeaddrinfo(result);

	if (int r = listen(m_Socket, SOMAXCONN); r == SOCKET_ERROR)
	{
		LOG("listen failed with error: " << WSAGetLastError());
		closesocket(m_Socket);
		WSACleanup();
		throw std::exception("listen failed");
	}
	else
		LOG("listen success");

	// Setup asynchronous socket for listening to new clients in the window message loop
	if (WSAAsyncSelect(m_Socket, m_hWnd, MSG_SERVER, FD_ACCEPT) == SOCKET_ERROR)
	{
		LOG("WSAAsyncSelect failed with error: " << WSAGetLastError());
		closesocket(m_Socket);
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
	if (m_Socket != INVALID_SOCKET)
	{
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
	// Fermez toutes les connexions avec les clients, si nécessaire

	for (Player player : m_Players)
	{
		closesocket(player.GetSocket());
	}
}

void Server::AcceptNewPlayer(Player newPlayer)
{
	if (newPlayer.GetSocket() == INVALID_SOCKET)
	{
		throw std::exception("accept failed");
	}
	else
		LOG("accept success");

	if (WSAAsyncSelect(newPlayer.GetSocket(), m_hWnd, MSG_CLIENT, FD_READ | FD_CLOSE) == SOCKET_ERROR)
	{
		LOG("WSAAsyncSelect failed with error: " << WSAGetLastError());
		closesocket(newPlayer.GetSocket());
		return;
	}
	else
		LOG("WSAAsyncSelect success");

	TGATPLAYERID playerId = newPlayer.GetId();
	m_Players.emplace(std::move(newPlayer));
	LOG("Number of clients: " << m_Players.size());

	nlohmann::json jsonData =
	{
		{"eventType", TgatServerMessage::PLAYER_INIT},
		{"playerId", playerId}
	};

	SendDataToPlayer(newPlayer, jsonData);
}

void Server::RemovePlayer(Player& player)
{
	nlohmann::json jsonData =
	{
		{"eventType", TgatServerMessage::PLAYER_DISCONNECT},
		{"message", "bye bye"}
	};

	SendDataToPlayer(player, jsonData);

	if (int r = closesocket(player.GetSocket()) != 0)
	{
		LOG("closesocket failed with error: " << r);
		return;
	}
	else
		LOG("closesocket success");
	// Retirez le client de la liste des clients

	m_Players.erase(player);
	LOG("Number of clients: " << m_Players.size());
}

void Server::RemovePlayer(SOCKET socket)
{
	auto it = std::find_if(m_Players.begin(), m_Players.end(), [socket](const Player& player)
		{
			return player.GetSocket() == socket;
		});


	if (it != m_Players.end())
	{
		nlohmann::json jsonData =
		{
			{"eventType", TgatServerMessage::PLAYER_DISCONNECT},
			{"message", "bye bye"}
		};

		SendDataToPlayer(*it, jsonData);

		if (int r = closesocket(it->GetSocket()); r != 0)
		{
			LOG("closesocket failed with error: " << r);
			return;
		}
		else
			LOG("closesocket success");

		m_Players.erase(it);
	}
}

void Server::SendDataToPlayer(const Player& player, nlohmann::json& data)
{
	try
	{
		TgatNetworkHelper::Message msg;
		std::string strData = data.dump();
		CreateMessage(HEADER_ID, player.GetId(), strData, msg);
		Send(player.GetSocket(), msg);
	}
	catch (const TgatException& e)
	{
		LOG(e.what());
	}
}

void Server::HandleJson(const nlohmann::json& json)
{
	LOG("Received JSON data: " << json.dump());
}

bool Server::SendToAllClients(const char* data, int size)
{
	//for (SOCKET clientSocket : m_ClientSockets)
	//{
	//	std::cout << m_ClientSockets.size() << std::endl;
	//	if (!SendToClient(clientSocket, data, size))
	//	{
	//		return false;
	//	}
	//}

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
			Server::GetInstance().AcceptNewPlayer(clientSocket);
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
			try
			{
				nlohmann::json jsonData = GetInstance().Receive((SOCKET)wParam);
				GetInstance().HandleJson(jsonData);
			}
			catch (TgatException& e)
			{
				LOG(e.what());
				Server::GetInstance().RemovePlayer(wParam);
				break;
			}

			break;
		}
		case FD_WRITE:
			break;
		case FD_CLOSE:
			Server::GetInstance().RemovePlayer(wParam);
			break;
		}
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // Call default message handler
}

bool Server::PlayerIdCheck(TGATPLAYERID playerId)
{
	// Check if the player id is in the list of connected players
	auto it = std::find_if(m_Players.begin(), m_Players.end(), [playerId](const Player& player)
		{
			return player.GetId() == playerId;
		});

	return it != m_Players.end();
}
