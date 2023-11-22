#pragma once

#include "Network/TgatNetworkHelper.h"

class NetworkManager : public TgatNetworkHelper
{
public:
	static NetworkManager& GetInstance();

	static const void DestroyInstance();

	bool Connect();
	void Disconnect();
	void Close();

	void HandleData(nlohmann::json& jsonData);
	void SendData(nlohmann::json&& jsonData);

	TGATPLAYERID GetPlayerId() const;
	TGATSESSIONID GetSessionId() const;

	const bool IsConnected() const { return m_Connected; }

	std::queue<nlohmann::json>& GetReceiveQueue(TgatServerMessage type) { return m_ReceiveQueues[type]; }
	bool ReceiveData(TgatServerMessage type, nlohmann::json& data);

private:
	TGATPLAYERID m_PlayerId;
	TGATSESSIONID m_SessionId;
	
	bool m_Connected;
	addrinfo m_AddressInfo;
	HWND m_hWnd;
	
	HANDLE m_NetworkThread;
	CRITICAL_SECTION m_SendCS;
	CRITICAL_SECTION m_ReceiveCS;
	std::queue<nlohmann::json> m_SendQueue;
	std::unordered_map<TgatServerMessage, std::queue<nlohmann::json>> m_ReceiveQueues;

	static NetworkManager* s_Instance;

private:
	NetworkManager();
	~NetworkManager() override;

	void Init();
	void InitWindow();

	void CreateSocket();

	bool PlayerIdCheck(TGATPLAYERID playerId) override;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ProcessMessages();

	void SendNetworkData();
	static DWORD WINAPI NetworkThread(LPVOID lpParam);
	void NetworkMain();
};