#include "Network/Http/HttpManager.h"
#include "Player/PlayerManager.h"
#include "Network/GameNetwork/GameNetworkManager.h"
#include "Game/GameManager.h"

#include "Server.h"

#include "Exceptions/TgatException.h"

Server::Server()
{
	m_HttpManager = new HttpManager();
	m_GameNetworkManager = new GameNetworkManager();
	m_PlayerManager = new PlayerManager();
	m_GameManager = new GameManager();

	InitWindow();
}

Server::~Server()
{
	CloseServer();

	WSACleanup();
}

void Server::StartServer()
{
	m_GameNetworkManager->Init();

	// Initialize Winsock
	WSADATA wsaData;
	if (int r = WSAStartup(MAKEWORD(2, 2), &wsaData); r != 0)
	{
		LOG("WSAStartup failed with error: " << r);
		throw std::exception("WSAStartup failed");
	}
	else
		LOG("WSAStartup success. Status: " << wsaData.szSystemStatus);

	SOCKET serverSocket = m_GameNetworkManager->GetSocket();
	char* port = m_GameNetworkManager->GetPort();
	InitSocket(serverSocket, m_hWnd, port, MSG_SERVER, FD_ACCEPT);
	port = nullptr;

	m_HttpManager->StartWebServer();
}

void Server::RunServer()
{
	ProcessMessages();
	m_HttpManager->CloseWebServer();
}

void Server::CloseServer()
{
	DELPTR(m_HttpManager);
	DELPTR(m_GameNetworkManager);

	// Close all sessions, this will also disconnect and delete all players in sessions
	m_GameManager->OnServerClose();
	DELPTR(m_GameManager);

	// Be sure to close all sessions first, otherwise, players will be deleted twice (in session and in player manager)
	DELPTR(m_PlayerManager);
}

void Server::InitSocket(SOCKET& s, HWND window, const char* port, uint32_t msgType, long events)
{
	addrinfo* result = nullptr;
	addrinfo hints{};

	// Resolve address and port
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE; // For wildcard IP address

	if (int r = getaddrinfo(nullptr, port, &hints, &result); r != 0)
	{
		LOG("getaddrinfo failed with error: " << r);
		WSACleanup();
		throw std::exception("getaddrinfo failed");
	}
	else
		LOG("getaddrinfo success");

	// Socket creation
	s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (s == INVALID_SOCKET)
	{
		LOG("socket failed with error: " << WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		throw std::exception("ServerSocket creation failed");
	}
	else
		LOG("socket success");

	// Bind socket to listen to TCP requests on the given address and port

	if (int r = bind(s, result->ai_addr, (int)result->ai_addrlen); r == SOCKET_ERROR)
	{
		LOG("bind failed with error: " << WSAGetLastError());
		freeaddrinfo(result);
		closesocket(s);
		WSACleanup();
		throw std::exception("bind failed");
	}
	else
		LOG("bind success");

	freeaddrinfo(result);
	result = nullptr;

	if (int r = listen(s, SOMAXCONN); r == SOCKET_ERROR)
	{
		LOG("listen failed with error: " << WSAGetLastError());
		closesocket(s);
		WSACleanup();
		throw std::exception("listen failed");
	}
	else
		LOG("listen success");

	// Setup asynchronous socket for listening to new clients in the window message loop
	if (WSAAsyncSelect(s, window, msgType, events) == SOCKET_ERROR)
	{
		LOG("WSAAsyncSelect failed with error: " << WSAGetLastError());
		closesocket(s);
		WSACleanup();
		throw std::exception("WSAAsyncSelect failed");
	}
	else
		LOG("WSAAsyncSelect server success");
}

void Server::AcceptNewPlayer(SOCKET socket)
{
	Player* newPlayer = new Player(socket);
	if (newPlayer->GetSocket() == INVALID_SOCKET)
	{
		throw std::exception("accept failed");
	}
	else
		LOG("accept success");

	if (WSAAsyncSelect(newPlayer->GetSocket(), m_hWnd, MSG_CLIENT, FD_READ | FD_CLOSE) == SOCKET_ERROR)
	{
		LOG("WSAAsyncSelect failed with error: " << WSAGetLastError());
		closesocket(newPlayer->GetSocket());
		throw std::exception("WSAAsyncSelect failed");
	}
	else
		LOG("WSAAsyncSelect success");

	nlohmann::json jsonData =
	{
		{JSON_EVENT_TYPE, TgatServerMessage::PLAYER_INIT},
		//{JSON_PLAYER_ID, (TGATPLAYERID)newPlayer->GetId()}
	};

	m_GameNetworkManager->SendDataToPlayer(newPlayer, jsonData);

	m_PlayerManager->AddPlayer(newPlayer);
}

void Server::HandleJson(const nlohmann::json& json)
{
	LOG("Received JSON data: " << json.dump());

	TgatClientMessage eventType = json[JSON_EVENT_TYPE].get<TgatClientMessage>();
	if (json.contains(JSON_PLAYER_ID) == false)
	{
		throw TgatException("Invalid JSON data, no JSON_PLAYER_ID field was found. Event type : " + (int)eventType);
	}

	GameSession* session = nullptr;
	nlohmann::json packageToSend;

	switch (eventType)
	{
	case TgatClientMessage::PLAYER_INPUT:
	{
		if (json.contains(JSON_PLAYER_MOVE) == false || json.contains(JSON_SESSION_ID) == false)
		{
			throw TgatException("Invalid JSON data, please add a JSON_PLAYER_MOVE and JSON_SESSION_ID field");
		}
		const TGATSESSIONID sessionId = json[JSON_SESSION_ID].get<TGATSESSIONID>();
		const nlohmann::json& move = json[JSON_PLAYER_MOVE];

		session = m_GameManager->GetActiveSessionById(sessionId);

		const TGATPLAYERID playerId = json[JSON_PLAYER_ID].get<TGATPLAYERID>();
		if (m_GameNetworkManager->PlayerIdCheck(playerId, session) == false)
		{
			throw TgatException("Player did not exist in this session");
		}

		session->Update(playerId, PLAYER_MOVE_ARGS(move), packageToSend);

		TgatServerMessage type = packageToSend[JSON_EVENT_TYPE].get<TgatServerMessage>();
		if (type == TgatServerMessage::BAD)
		{
			m_GameNetworkManager->SendDataToPlayer(m_PlayerManager->GetPlayerById(playerId), packageToSend);
			break;
		}

		if (session != nullptr)
			m_GameNetworkManager->SendDataToAllPlayersInSession(session, packageToSend);
		else
		{
			throw TgatException("No session found");
		}

		break;
	}
	case TgatClientMessage::CREATE_SESSION:
	{
		const TGATPLAYERID playerId = json[JSON_PLAYER_ID].get<TGATPLAYERID>();
		Player* p1 = m_PlayerManager->GetPlayerById(playerId);
		if (p1 == nullptr)
		{
			throw TgatException("Invalid player Id");
		}

		session = m_GameManager->CreateGameSession(p1);

		packageToSend =
		{
			{JSON_EVENT_TYPE, TgatServerMessage::SESSION_CREATED},
			{JSON_SESSION_ID, (TGATSESSIONID)session->GetId()},
		};

		m_GameNetworkManager->SendDataToPlayer(p1, packageToSend);

		break;
	}
	case TgatClientMessage::JOIN_SESSION:
	{
		const TGATSESSIONID sessionId = json[JSON_SESSION_ID].get<TGATSESSIONID>();
		const TGATPLAYERID playerId = json[JSON_PLAYER_ID].get<TGATPLAYERID>();

		Player* p2 = m_PlayerManager->GetPlayerById(playerId);
		if (p2 == nullptr)
		{
			throw TgatException("Invalid player Id");
		}

		session = m_GameManager->GetWaitingSessionById(sessionId);
		if (session == nullptr)
		{
			packageToSend =
			{
				{JSON_EVENT_TYPE, TgatServerMessage::BAD_SESSION_ID}
			};
			m_GameNetworkManager->SendDataToPlayer(p2, packageToSend);
			break;
		}

		m_GameManager->AddPlayerToGameSession(p2, sessionId);

		packageToSend =
		{
			{JSON_EVENT_TYPE, TgatServerMessage::SESSION_JOINED},
			{JSON_SESSION_ID, (TGATSESSIONID)session->GetId()},
		};

		m_GameNetworkManager->SendDataToAllPlayersInSession(session, packageToSend);

		break;
	}
	}
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

void Server::ProcessMessages()
{
	MSG msg{};
	while (true)
	{
		if (_kbhit() && _getch() == VK_ESCAPE)
		{
			return;
		}

		// Peek message
		while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Add a sleep to avoid busy-waiting
		Sleep(1);
	}
}

LRESULT Server::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
#pragma region ServerMessage
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
		{
			SOCKET clientSocket = accept(wParam, nullptr, nullptr);
			try
			{
				I(Server).AcceptNewPlayer(clientSocket); // Implicit construction of Player
			}
			catch (std::exception& e)
			{
				LOG(e.what());
				break;
			}
			catch (...)
			{
				LOG("Unknown exception");
				break;
			}
			break;
		}
		case FD_CLOSE:
		{
			LOG("FD_CLOSE");

			break;
		}
		}

		return 0;
	}
#pragma endregion
#pragma region ClientMessage
	case MSG_CLIENT:
	{
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
				nlohmann::json jsonData;
				if (I(Server).GetGameNetworkManager()->Receive((SOCKET)wParam, jsonData) == WSAEWOULDBLOCK)
					LOG("WSAEWOULDBLOCK");
				else
					I(Server).HandleJson(jsonData);
			}
			catch (TgatException& e)
			{
				LOG(e.what());
				I(Server).GetPlayerManager()->RemovePlayer(wParam);
				break;
			}
			catch (nlohmann::json::exception& e)
			{
				LOG(e.what());
				I(Server).GetPlayerManager()->RemovePlayer(wParam);
				break;
			}
			catch (...)
			{
				LOG("Unknown exception");
				I(Server).GetPlayerManager()->RemovePlayer(wParam);
				break;
			}

			break;
		}
		case FD_WRITE:
			break;
		case FD_CLOSE:
			I(Server).GetPlayerManager()->RemovePlayer(wParam);
			break;
		}
		return 0;
	}
#pragma endregion
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // Call default message handler
}