#include "NetworkManager.h"

#include "Exceptions/TgatException.h"

#define MSG_SERVER (WM_USER + 1)
#define MSG_SEND (WM_APP)

static constexpr char PORT[5] = "6969";

NetworkManager* NetworkManager::s_Instance = nullptr;

NetworkManager::NetworkManager() : m_hWnd(nullptr), m_PlayerId(0)
{
	Init();
}

NetworkManager& NetworkManager::GetInstance()
{
	if (s_Instance == nullptr)
		s_Instance = new NetworkManager();
	return *s_Instance;
}

const void NetworkManager::DestroyInstance()
{
	if (s_Instance == nullptr)
		return;

	DELPTR(s_Instance);
}

NetworkManager::~NetworkManager()
{
	// Close socket
	Disconnect();

	// Cleanup Winsock
	WSACleanup();
}

bool NetworkManager::Connect()
{
	if (m_ServerSocket == INVALID_SOCKET)
		CreateSocket();

	// Connect to server
	if (connect(m_ServerSocket, m_AddressInfo.ai_addr, (int)m_AddressInfo.ai_addrlen) == SOCKET_ERROR)
	{
		LOG("connect failed with error: " << WSAGetLastError());
		closesocket(m_ServerSocket);
		m_ServerSocket = INVALID_SOCKET;
		return m_Connected;
	}
	else
		LOG("connect success");

	if (WSAAsyncSelect(m_ServerSocket, m_hWnd, MSG_SERVER, FD_READ | FD_CLOSE))
	{
		LOG("WSAAsyncSelect failed with error: " << WSAGetLastError());
		closesocket(m_ServerSocket);
		m_ServerSocket = INVALID_SOCKET;
		return m_Connected;
	}
	else
		LOG("WSAAsyncSelect success");

	m_Connected = true;

	return m_Connected;
}

void NetworkManager::Disconnect()
{
	// Disconnect from server
	closesocket(m_ServerSocket);
	m_Connected = false;
}

void NetworkManager::HandleData(nlohmann::json& data)
{
	// Check if the data has a valid type
	if (data.contains("eventType"))
	{
		switch ((TgatServerMessage)data["eventType"])
		{
		case TgatServerMessage::PLAYER_INIT:
			m_PlayerId = data["playerId"];
			LOG("Player ID: " << m_PlayerId);
			break;
		default:
			break;
		}
	}
}

void NetworkManager::SendData(nlohmann::json&& jsonData)
{
	// emplace data in queue on the main thread
	// Enter critical section
	EnterCriticalSection(&m_CriticalSection);	
	
	m_EventQueue.emplace(std::move(jsonData));
	PostMessage(m_hWnd, MSG_SEND, 0, 0);

	// Leave critical section
	LeaveCriticalSection(&m_CriticalSection);
}

TGATPLAYERID NetworkManager::GetPlayerId() const
{
	 return (TGATPLAYERID)m_PlayerId;
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

	CreateSocket();
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

void NetworkManager::CreateSocket()
{
	// Resolve address and port
	addrinfo* result = NULL;
	addrinfo hints{};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult = getaddrinfo("localhost", PORT, &hints, &result);

	if (iResult != 0)
	{
		LOG("getaddrinfo failed with error: " << iResult);
		m_ServerSocket = INVALID_SOCKET;
		return;
	}
	else
		LOG("getaddrinfo success");

	// Socket creation
	m_ServerSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_ServerSocket == INVALID_SOCKET)
	{
		LOG("socket failed with error: " << WSAGetLastError());
		freeaddrinfo(result);
		m_ServerSocket = INVALID_SOCKET;
		return;
	}
	else
		LOG("socket success");

	m_AddressInfo = *result;
	freeaddrinfo(result);
}

LRESULT NetworkManager::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case MSG_SEND:
	{
		GetInstance().SendNetworkData();
		return 0;
    }
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
			try
			{
				nlohmann::json jsonData;
				if (GetInstance().Receive((SOCKET)wParam, jsonData) == WSAEWOULDBLOCK)
                    LOG("WSAEWOULDBLOCK");
				else
					GetInstance().HandleData(jsonData);
			}
			catch (TgatException& e)
			{
				LOG(e.what());
			}
			break;
		}
		case FD_CLOSE:
		{
			LOG("FD_CLOSE");
			GetInstance().Disconnect();
			break;
		}
		}

		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void NetworkManager::SendNetworkData()
{
	// send data on network thread
	EnterCriticalSection(&m_CriticalSection);
	TgatNetworkHelper::Message msg;
	std::string strData = m_EventQueue.front().dump();
	const int headerId = HEADER_ID;
	const int playerId = GetPlayerId();
	CreateMessage(headerId, playerId, strData, msg);
	Send(msg);
	m_EventQueue.pop();
	LeaveCriticalSection(&m_CriticalSection);
}

bool NetworkManager::PlayerIdCheck(TGATPLAYERID playerId)
{
	// Will always return true for now on the client side. We know that there won't be any other server sending us data.
	return true;
}