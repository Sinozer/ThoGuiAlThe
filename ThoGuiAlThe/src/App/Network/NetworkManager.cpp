#include "NetworkManager.h"

#define MSG_SERVER (WM_USER + 1)

static constexpr char PORT[5] = "6969";

NetworkManager* NetworkManager::s_Instance = nullptr;

NetworkManager& NetworkManager::GetInstance()
{
	if (s_Instance == nullptr)
		s_Instance = new NetworkManager();
	return *s_Instance;
}

void NetworkManager::Destroy()
{
	// Close socket
	Disconnect();

	// Cleanup Winsock
	WSACleanup();
}

void NetworkManager::Connect()
{
	// Connect to server
	if (connect(m_Socket, m_AddressInfo.ai_addr, (int)m_AddressInfo.ai_addrlen) == SOCKET_ERROR)
	{
		LOG("connect failed with error: " << WSAGetLastError());
		closesocket(m_Socket);
		WSACleanup();
	}
	else
		LOG("connect success");

	if (WSAAsyncSelect(m_Socket, m_hWnd, MSG_SERVER, FD_READ | FD_CLOSE))
	{
		LOG("WSAAsyncSelect failed with error: " << WSAGetLastError());
		closesocket(m_Socket);
		WSACleanup();
	}
	else
		LOG("WSAAsyncSelect success");
}

void NetworkManager::Disconnect()
{
	// Disconnect from server
	closesocket(m_Socket);
}

void NetworkManager::SendData(std::string data)
{
	// Send data
	if (send(m_Socket, data.c_str(), (int)data.size(), 0) == SOCKET_ERROR)
	{
		LOG("send failed with error: " << WSAGetLastError());
		closesocket(m_Socket);
		WSACleanup();
	}
	else
		LOG("send success");
}

void NetworkManager::SendData(nlohmann::json& data)
{
	data.emplace("playerId", m_PlayerId);
	// Serialize the JSON data to a string
	std::string serializedData = data.dump();

	// Send data
	if (send(m_Socket, serializedData.c_str(), static_cast<int>(serializedData.size()), 0) == SOCKET_ERROR)
	{
		LOG("send failed with error: " << WSAGetLastError());
		closesocket(m_Socket);
		WSACleanup();
	}
	else
		LOG("send success");
}

void NetworkManager::HandleData(nlohmann::json& data)
{
	// Check if the data has a valid type
	if (data.contains("eventType"))
	{
		if (data["eventType"] == "INIT_PLAYER")
		{
			m_PlayerId = data["playerId"];
			LOG("Player ID: " << m_PlayerId);
		}
	}
}

void NetworkManager::Init()
{
	InitWindow();
	// Initialize network
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		LOG("WSAStartup failed with error: " << iResult);
		// TODO: Throw exception
		throw std::exception("WSAStartup failed");
	}
	else
		LOG("WSAStartup success. Status: " << wsaData.szSystemStatus);

	// Resolve address and port
	addrinfo* result = NULL;
	addrinfo hints{};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo("localhost", PORT, &hints, &result);

	if (iResult != 0)
	{
		LOG("getaddrinfo failed with error: " << iResult);
		WSACleanup();
		// TODO: Throw exception
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
		// TODO: Throw exception
		throw std::exception("socket creation failed");
	}
	else
		LOG("socket success");

	m_AddressInfo = *result;
	freeaddrinfo(result);
}

void NetworkManager::InitWindow()
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

LRESULT NetworkManager::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		case FD_READ:
		{
			char buffer[4096];  // Adjust the buffer size as needed
			int bytesReceived = recv((SOCKET)wParam, buffer, sizeof(buffer), 0);
			if (bytesReceived == SOCKET_ERROR)
			{
				LOG("recv failed with error: " << WSAGetLastError());
				break;
			}
			else
			{
				buffer[bytesReceived] = '\0';

				// Parse the received JSON data
				try
				{
					nlohmann::json jsonData = nlohmann::json::parse(buffer);
					GetInstance().HandleData(jsonData);
				}
				catch (const nlohmann::json::exception& e)
				{
					LOG("Error parsing JSON: " << e.what());
					break;
				}
			}

			break;
			}
		}

		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}