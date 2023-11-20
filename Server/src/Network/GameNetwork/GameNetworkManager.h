#pragma once

#include "Network/TgatNetworkHelper.h"

class Player;

class GameNetworkManager : public TgatNetworkHelper
{
public:
	GameNetworkManager();
	~GameNetworkManager() override;

public:
	void Init();
	void SendDataToPlayer(const Player& player, nlohmann::json& data);
	void SendDataToAllPlayers(nlohmann::json& data);
	void SendDataToAllPlayersExcept(const Player& player, nlohmann::json& data);

	void HandleJson(const nlohmann::json& json);

private:
	char m_Port[5];
	HWND m_hWnd;

	void InitWindow();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

