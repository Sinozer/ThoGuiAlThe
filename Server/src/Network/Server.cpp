#include "Server.h"
#include "Player.h"

#define MSG_SERVER (WM_USER + 1)
#define MSG_CLIENT (WM_USER + 2)
#define MSG_WEB (WM_USER + 3)

Server::Server() : m_hWnd(nullptr), m_Port{ "6969" }, m_WebPort{ "9669" }, m_ServerSocket(INVALID_SOCKET)
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

	m_Players.emplace(std::move(newPlayer));
	LOG("Number of clients: " << m_Players.size());

	nlohmann::json jsonData = 
	{ 
		{"eventType", "INIT_PLAYER"},
		{"playerId", newPlayer.GetId()}
	};

	SendDataToPlayer(newPlayer, jsonData);
}

void Server::RemovePlayer(Player& player)
{
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

void Server::SendDataToPlayer(const Player& player, const nlohmann::json& data)
{
	std::string dataString = data.dump();
	size_t size = dataString.size();

if (int r = send(player.GetSocket(), dataString.c_str(), size, 0); r == SOCKET_ERROR)
	{
		LOG("send failed with error: " << WSAGetLastError());
	}
	else
		LOG("send success");
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
	std::string path;
	std::string httpVersion;

	ss >> method >> path >> httpVersion;

	// Process the HTTP request (this is where you would typically handle the request)
	if (method == "GET")
	{
		if (path == "/")
		{
			// Send the HTML file
			std::ifstream file("assets/index.html");
			std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

			std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(fileContent.size()) + "\r\n\r\n" + fileContent;

			if (int r = send(socket, response.c_str(), response.size(), 0); r == SOCKET_ERROR)
			{
				LOG("send failed with error: " << WSAGetLastError());
			}
			else
				LOG("send success");
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
			char buffer[4096];  // Adjust the buffer size as needed
			int bytesReceived = recv((SOCKET)wParam, buffer, sizeof(buffer), 0);
			if (bytesReceived == SOCKET_ERROR)
			{
				LOG("recv failed with error: " << WSAGetLastError());
				Server::GetInstance().RemovePlayer(wParam);
				break;
			}
			else
			{
				buffer[bytesReceived] = '\0';

				// Parse the received JSON data
				try
				{
					nlohmann::json jsonData = nlohmann::json::parse(buffer);
					Server::GetInstance().HandleJson(jsonData);
				}
				catch (const nlohmann::json::exception& e)
				{
					LOG("Error parsing JSON: " << e.what());
					std::unordered_set<Player>& players = Server::GetInstance().GetPlayers();

					Server::GetInstance().RemovePlayer(wParam);
				}
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
			// Process the HTTP request (this is where you would typically handle the request)
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
