#pragma once

#include "Network/TgatNetworkHelper.h"

class NetworkManager : public TgatNetworkHelper
{
public:
	static NetworkManager& GetInstance();

	static void DestroyInstance()
	{
		if (s_Instance != nullptr)
			s_Instance->Destroy();
	}

	void Destroy();

	void Connect();
	void Disconnect();

	void HandleData(nlohmann::json& jsonData);

	uint32_t GetPlayerId() const;

private:
	addrinfo m_AddressInfo;
	HWND m_hWnd;
	uint32_t m_PlayerId;

	static NetworkManager* s_Instance;

private:
	NetworkManager();
	~NetworkManager() override;

	void Init();
	void InitWindow();

	bool PlayerIdCheck(TGATPLAYERID playerId) override;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};