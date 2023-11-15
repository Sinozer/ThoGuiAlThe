#include "NetworkManager.h"

static constexpr char PORT[5] = "6969";

void NetworkManager::Init()
{
	// Initialize network
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		LOG("WSAStartup failed with error: " << iResult);
		// TODO: Throw exception
		assert(false);
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
		assert(false);
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
		assert(false);
	}
	else
		LOG("socket success");

	m_AddressInfo = *result;
	freeaddrinfo(result);
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
}

void NetworkManager::Disconnect()
{
	// Disconnect from server
	closesocket(m_Socket);
}

void NetworkManager::SendData()
{
	// Send data
	char sendbuf[256] = "this is a test";
	int iResult = send(m_Socket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR)
	{
		LOG("send failed with error: " << WSAGetLastError());
		closesocket(m_Socket);
		WSACleanup();
	}
	else
		LOG("send success");
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

void NetworkManager::ReceiveData()
{}
