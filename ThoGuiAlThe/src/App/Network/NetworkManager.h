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

private:
	bool m_Connected = false;

	addrinfo m_AddressInfo;
	HWND m_hWnd;
	TGATPLAYERID m_PlayerId;
	TGATSESSIONID m_SessionId;

	static NetworkManager* s_Instance;

private:
	NetworkManager();
	~NetworkManager() override;

	void Init();
	void InitWindow();

	void CreateSocket();

	bool PlayerIdCheck(TGATPLAYERID playerId) override;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};