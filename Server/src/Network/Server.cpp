#include "Server.h"

#include "Exceptions/TgatException.h"
#include "Player.h"

#define MSG_SERVER (WM_USER + 1)
#define MSG_CLIENT (WM_USER + 2)
#define MSG_WEB (WM_USER + 3)

Server::Server() : TgatNetworkHelper(), m_hWnd(nullptr), m_Port{ "6969" }, m_WebPort{ "9669" }
{}

Server::~Server()
{
	CloseServer();

	WSACleanup();
}

void Server::StartServer()
{
	InitWindow();
	InitHttpRequestHandlers();
	
	// Initialize Winsock
	WSADATA wsaData;
	if (int r = WSAStartup(MAKEWORD(2, 2), &wsaData); r != 0)
	{
		LOG("WSAStartup failed with error: " << r);
		throw std::exception("WSAStartup failed");
	}
	else
		LOG("WSAStartup success. Status: " << wsaData.szSystemStatus);

	// Initialize the server socket
	InitSocket(m_ServerSocket, m_Port, MSG_SERVER, FD_ACCEPT);
	InitSocket(m_WebServerSocket, m_WebPort, MSG_WEB, FD_ACCEPT | FD_CLOSE | FD_READ);
}

void Server::InitSocket(SOCKET& s, const char* port, uint32_t msgType, long events)
{
	addrinfo* result = nullptr;
	addrinfo hints{};

	// Resolve address and port
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE; // For wildcard IP address

	if (int r = getaddrinfo("localhost", port, &hints, &result); r != 0)
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
		closesocket(m_ServerSocket);
		WSACleanup();
		throw std::exception("bind failed");
	}
	else
		LOG("bind success");

	freeaddrinfo(result);
	result = nullptr;

	if (int r = listen(m_ServerSocket, SOMAXCONN); r == SOCKET_ERROR)
	{
		LOG("listen failed with error: " << WSAGetLastError());
		closesocket(s);
		WSACleanup();
		throw std::exception("listen failed");
	}
	else
		LOG("listen success");

	// Setup asynchronous socket for listening to new clients in the window message loop
	if (WSAAsyncSelect(s, m_hWnd, msgType, events) == SOCKET_ERROR)
	{
		LOG("WSAAsyncSelect failed with error: " << WSAGetLastError());
		closesocket(s);
		WSACleanup();
		throw std::exception("WSAAsyncSelect failed");
	}
	else
		LOG("WSAAsyncSelect server success");
}

void Server::InitHttpRequestHandlers()
{
	m_HttpRequestHandlers.insert({ "/", std::make_unique<HomeRequestHandler>() });
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

		// Your other processing logic can go here

		// Add a sleep to avoid busy-waiting
		Sleep(1);
	}
}

void Server::CloseServer()
{
	if (m_ServerSocket != INVALID_SOCKET)
	{
		closesocket(m_ServerSocket);
		m_ServerSocket = INVALID_SOCKET;
	}
	// Fermez toutes les connexions avec les clients, si n�cessaire

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

void Server::HandleHttpRequest(std::string request, SOCKET socket)
{
	// Parse the HTTP request
	std::stringstream ss(request);
	std::string method;
	std::string url;
	std::string httpVersion;

	ss >> method >> url >> httpVersion;

	// Extracting parameters from the URL
	size_t paramsStart = url.find('?');
	std::string route = (paramsStart != std::string::npos) ? url.substr(0, paramsStart) : url;

	// Extracting parameters into a map
	std::unordered_map<std::string, std::string> params;
	if (paramsStart != std::string::npos)
	{
		params = RequestHandler::ParseParams(url.substr(paramsStart + 1));
	}

	// Process the HTTP request (this is where you would typically handle the request)
	std::string response;
	if (m_HttpRequestHandlers.contains(route) == true)
		response = m_HttpRequestHandlers[route]->HandleHttpRequest(params, method);
	else
		response = RequestHandler::NotFound();
	
	if (int r = send(socket, response.c_str(), (int)response.size(), 0); r == SOCKET_ERROR)
	{
		LOG("send failed with error: " << WSAGetLastError());
	}
	else
		LOG("send success");
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
			{
				SOCKET clientSocket = accept(wParam, nullptr, nullptr);
				Server::GetInstance().AcceptNewPlayer(clientSocket);
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
				nlohmann::json jsonData;
				if (GetInstance().Receive((SOCKET)wParam, jsonData) == WSAEWOULDBLOCK)
					LOG("WSAEWOULDBLOCK");
				else
					GetInstance().HandleJson(jsonData);
			}
			catch (TgatException& e)
			{
				LOG(e.what());
				Server::GetInstance().RemovePlayer(wParam);
				break;
			}
			catch (nlohmann::json::exception& e)
			{
				LOG(e.what());
				Server::GetInstance().RemovePlayer(wParam);
				break;
			}
			catch (...)
			{
				LOG("Unknown exception");
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
	case MSG_WEB:
		if (WSAGETSELECTERROR(lParam))
		{
			LOG("FD_READ failed with error: " << WSAGetLastError());
			break;
		}

		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_ACCEPT:
		{
			SOCKET webClientSocket = accept(wParam, nullptr, nullptr);
			if (webClientSocket == INVALID_SOCKET)
			{
				LOG("accept failed with error: " << WSAGetLastError());
				break;
			}
			break;
		}
		case FD_READ:
		{
			char buffer[4096];
			int bytesReceived = recv((SOCKET)wParam, buffer, sizeof(buffer), 0);

			if (bytesReceived == SOCKET_ERROR)
			{
				LOG("recv failed with error: " << WSAGetLastError());
				closesocket((SOCKET)wParam);
				break;
			}
			else if (bytesReceived == 0)
			{
				// Connection closed by the client
				closesocket((SOCKET)wParam);
				LOG("Web client disconnected.");
				break;
			}

			buffer[bytesReceived] = '\0';

			// Parse the received HTTP request
			std::string httpRequest(buffer);
			// Process the HTTP request
			GetInstance().HandleHttpRequest(httpRequest, (SOCKET)wParam);

			break;
		}
		case FD_CLOSE:
			if (WSAGETSELECTERROR(lParam))
			{
				LOG("FD_READ failed with error: " << WSAGetLastError());
				break;
			}

			closesocket(wParam);

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
