#pragma once

#include "Network/TgatNetworkHelper.h"
#include "Player/Player.h"

class GameNetworkManager : public TgatNetworkHelper
{
public:
	GameNetworkManager();
	~GameNetworkManager() override;

public:
	[[nodiscard]] SOCKET& GetSocket() { return m_ServerSocket; }
	[[nodiscard]] char* GetPort() { return m_Port; }

	void Init();
	void SendDataToPlayer(const Player& player, nlohmann::json& data);
	void SendDataToAllPlayers(std::unordered_set<Player>& players, nlohmann::json& data);

private:
	char m_Port[5];

	SOCKET m_ServerSocket;

	bool PlayerIdCheck(TGATPLAYERID playerId) override;
};

