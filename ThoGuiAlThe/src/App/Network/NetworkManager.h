#pragma once

#include "Network/TgatNetworkHelper.h"

class User;

class NetworkManager : public TgatNetworkHelper
{
public:
	static NetworkManager& GetInstance();

	static const void DestroyInstance();

	bool Connect(std::string ipAddress = "localhost");
	void Disconnect();

	void HandleData(nlohmann::json& jsonData);

	void InitPlayerWithData(nlohmann::json& jsonData);

	TGATPLAYERID GetPlayerId() const;
	const PlayerDisplayData& GetPlayerDisplayData() const;
	User* GetUser() const { return m_User; }
	TGATSESSIONID GetSessionId() const;

	const bool IsConnected() const { return m_Connected; }
	const bool IsInit() const { return m_User != nullptr; }

	std::queue<nlohmann::json>& GetReceiveQueue(TgatServerMessage type) { return m_ReceiveQueues[type]; }

private:
	bool m_Connected = false;

	addrinfo m_AddressInfo;
	HWND m_hWnd;

	User* m_User;

	TGATSESSIONID m_SessionId;
	std::unordered_map<TgatServerMessage, std::queue<nlohmann::json>> m_ReceiveQueues;

	static NetworkManager* s_Instance;

private:
	NetworkManager();
	~NetworkManager() override;

	void Init();
	void InitWindow();

	void CreateSocket(std::string ipAddress = "localhost");

	bool PlayerIdCheck(TGATPLAYERID playerId) override;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};