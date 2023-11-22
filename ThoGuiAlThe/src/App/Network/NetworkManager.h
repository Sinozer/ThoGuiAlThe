#pragma once

#include "Network/TgatNetworkHelper.h"

class NetworkManager : public TgatNetworkHelper
{
public:
	static NetworkManager& GetInstance();

	static const void DestroyInstance();

	bool Connect();
	void Disconnect();

	void HandleData(nlohmann::json& jsonData);

	TGATPLAYERID GetPlayerId() const;
	TGATSESSIONID GetSessionId() const;

	const bool IsConnected() const { return m_Connected; }

	std::queue<nlohmann::json>& GetReceiveQueue(TgatServerMessage type) { return m_ReceiveQueues[type]; }

private:
	bool m_Connected = false;

	addrinfo m_AddressInfo;
	HWND m_hWnd;
	TGATPLAYERID m_PlayerId;
	TGATSESSIONID m_SessionId;
	std::unordered_map<TgatServerMessage, std::queue<nlohmann::json>> m_ReceiveQueues;

	HANDLE m_NetworkHandle;

	static NetworkManager* s_Instance;

private:
	NetworkManager();
	~NetworkManager() override;

	void StartNetworkServer();

	void Init();
	void InitWindow();

	void CreateSocket();

	bool PlayerIdCheck(TGATPLAYERID playerId) override;

	static DWORD WINAPI NetworkThread(LPVOID lpParam);

	void NetworkMain();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};