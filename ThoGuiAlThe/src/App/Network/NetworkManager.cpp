#include "App/User/User.h"

#include "NetworkManager.h"

#include "App/State/List/Connect/ConnectState.h"
#include "App/State/List/Create/CreateState.h"
#include "App/State/List/Game/GameState.h"
#include "App/State/List/Result/ResultState.h"

#include "Exceptions/TgatException.h"

#define MSG_SEND (WM_APP)

static constexpr char PORT[5] = "6969";

NetworkManager* NetworkManager::s_Instance = nullptr;

NetworkManager::NetworkManager() 
	: TgatNetworkHelper(), m_SessionId(-1), m_Connected(false),
	m_AddressInfo{}, m_NetworkThread(), m_SendCS{}, m_ReceiveCS{}, m_SendQueue(), m_ReceiveQueues()
{
	m_hWnd = nullptr;
	m_User = nullptr;
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

	DeleteCriticalSection(&m_SendCS);
	DeleteCriticalSection(&m_ReceiveCS);
}

bool NetworkManager::Connect(std::string ipAddress)
{
	if (m_ServerSocket == INVALID_SOCKET)
		CreateSocket(ipAddress);

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
	DELPTR(m_User);
}

void NetworkManager::Close()
{
	SendMessage(m_hWnd, MSG_NUKE, 0, 0);
	m_NetworkThread.Join();
	UnregisterClass(L"ServerWindow", GetModuleHandle(nullptr));
}

void NetworkManager::HandleData(nlohmann::json& data)
{
	if (data.contains(JSON_EVENT_TYPE) == false)
	{
		LOG("JSON_EVENT_TYPE not found");
		return;
	}

	TgatServerMessage type = data[JSON_EVENT_TYPE].get<TgatServerMessage>();
	CriticalSectionScope csScope{ m_ReceiveCS };
	switch (type)
	{
	case TgatServerMessage::PLAYER_DISCONNECT:
	{
		m_ReceiveQueues[TgatServerMessage::PLAYER_DISCONNECT].push(data);
		break;
	}
	case TgatServerMessage::PLAYER_INIT:
	{
		m_ReceiveQueues[TgatServerMessage::PLAYER_INIT].push(data);
		break;
	}
	case TgatServerMessage::SESSION_CREATED:
	{
		m_SessionId = data[JSON_SESSION_ID];
		LOG(JSON_SESSION_ID << ": " << m_SessionId);
		StateManager::GetInstance()->AddState(new CreateState());
		break;
	}
	case TgatServerMessage::SESSION_JOINED:
	{
		m_SessionId = data[JSON_SESSION_ID];
		LOG(JSON_SESSION_ID << ": " << m_SessionId);
		StateManager* stateManager = I(StateManager);
		stateManager->GoToFirstState();
		stateManager->AddState(new GameState());
		break;
	}
	case TgatServerMessage::SESSION_LEFT:
	{
		if (data[JSON_SESSION_ID] != GetSessionId() && data[JSON_SESSION_ID] != -1)
			return;

		m_ReceiveQueues[TgatServerMessage::SESSION_LEFT].push(data);

		m_SessionId = -1;
		break;
	}
	case TgatServerMessage::PLAYER_INPUT:
	{
		m_ReceiveQueues[TgatServerMessage::PLAYER_INPUT].push(data);
		break;
	}
	case TgatServerMessage::GAME_END:
	{
		m_ReceiveQueues[TgatServerMessage::GAME_END].push(data);
		I(StateManager)->GoToFirstState();
		I(StateManager)->AddState(new ResultState());
		break;
	}
	case TgatServerMessage::PLAYER_INFO_CHANGED:
	{
		m_ReceiveQueues[TgatServerMessage::PLAYER_INFO_CHANGED].push(data);
		break;
	}
	case TgatServerMessage::GAME_REPLAY:
	{
		m_ReceiveQueues[TgatServerMessage::GAME_REPLAY].push(data);
		break;
	}
	case TgatServerMessage::BAD_SESSION_ID:
	{
		m_ReceiveQueues[TgatServerMessage::BAD_SESSION_ID].push(data);
		break;
	}
	case TgatServerMessage::PLAYER_INFO:
	{
		m_ReceiveQueues[TgatServerMessage::PLAYER_INFO].push(data);
		break;
	}
	default:
		break;
	}
}

void NetworkManager::SendData(nlohmann::json&& jsonData)
{
	// Enter critical section
	CriticalSectionScope csScope{ m_SendCS };
	
	m_SendQueue.emplace(std::move(jsonData));
	PostMessage(m_hWnd, MSG_SEND, 0, 0);
}

bool NetworkManager::ReceiveData(TgatServerMessage type, nlohmann::json& data)
{
	CriticalSectionScope csScope{ m_ReceiveCS };
	if (m_ReceiveQueues[type].empty())
	{
		return false;
	}
	data = nlohmann::json(std::move(m_ReceiveQueues[type].front()));
	m_ReceiveQueues[type].pop();
	return true;
}

TGATPLAYERID NetworkManager::GetPlayerId() const
{
	return m_User->GetId();
}

const PlayerDisplayData& NetworkManager::GetPlayerDisplayData() const
{
	return m_User->GetDisplayData();
}

TGATSESSIONID NetworkManager::GetSessionId() const
{
	return (TGATSESSIONID)m_SessionId;
}

void NetworkManager::CreateSocket(std::string ipAddress)
{
	// Resolve address and port
	addrinfo* result = NULL;
	addrinfo hints{};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult = getaddrinfo(ipAddress.c_str(), PORT, &hints, &result);

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

void NetworkManager::Init()
{
	InitializeCriticalSection(&m_SendCS);
	InitializeCriticalSection(&m_ReceiveCS);

	// Create network thread
	m_NetworkThread = TgatThread([this] { NetworkMain(); });
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
	case MSG_NUKE:
	{
		DestroyWindow(GetInstance().m_hWnd);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case MSG_SEND:
	{
		GetInstance().SendNetworkData();
		return 0;
	}
	case MSG_SERVER:
	{
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
				UNREFERENCED_PARAMETER(e);
				LOG(e.what());
			}
			break;
		}
		case FD_CLOSE:
		{
			LOG("FD_CLOSE");
			GetInstance().Disconnect();
			StateManager* stateManager = I(StateManager);
			stateManager->RemoveAllStates();
			stateManager->AddState(new ConnectState());
			break;
		}
		}

		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void NetworkManager::InitPlayerWithData(nlohmann::json& jsonData)
{
	m_User = new User(jsonData[JSON_PLAYER_ID]);
	LOG("Self was created with id : " << m_User->GetId());

	if ((jsonData.contains(JSON_PLAYER_NAME) || jsonData.contains(JSON_PLAYER_PPP) || jsonData.contains(JSON_PLAYER_COLOR)) == false)
	{
		LOG("JSON_PLAYER_NAME or JSON_PLAYER_PPP or JSON_PLAYER_COLOR not found");
		return;
	}

	PlayerDisplayData data;
	data.name = PLAYER_DD_ARG_NAME(jsonData);
	data.profilePicturePath = PLAYER_DD_ARG_PPP(jsonData);
	data.profilePictureThumbPath = PLAYER_DD_ARG_PPTP(jsonData);
	data.color = PLAYER_DD_ARG_COLOR(jsonData);
	m_User->SetDisplayData(data);
}

void NetworkManager::ProcessMessages()
{
	MSG msg{};
	while (GetMessage(&msg, m_hWnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void NetworkManager::SendNetworkData()
{
	// send data on network thread
	CriticalSectionScope csScope{ m_SendCS };
	TgatNetworkHelper::Message msg;
	std::string strData = m_SendQueue.front().dump();
	const int headerId = HEADER_ID;
	const int playerId = GetPlayerId();
	CreateMessage(headerId, playerId, strData, msg);
	Send(msg);
	m_SendQueue.pop();
}

void NetworkManager::NetworkMain()
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

	//Connect();

	ProcessMessages();

	// Close socket
	Disconnect();
	// Cleanup Winsock
	WSACleanup();
}

bool NetworkManager::PlayerIdCheck(TGATPLAYERID playerId)
{
	// Will always return true for now on the client side. We know that there won't be any other server sending us data.
	return true;
}
