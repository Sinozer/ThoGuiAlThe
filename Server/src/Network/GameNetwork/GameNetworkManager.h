#pragma once

#include "Network/TgatNetworkHelper.h"
#include "Player/Player.h"

class GameSession;

class GameNetworkManager : public TgatNetworkHelper
{
	friend class Server;
public:
	GameNetworkManager();
	~GameNetworkManager() override;

public:
	[[nodiscard]] SOCKET& GetSocket() { return m_ServerSocket; }
	[[nodiscard]] char* GetPort() { return m_Port; }

	void Init();
	void SendDataToPlayer(const Player& player, nlohmann::json& data);
	void SendDataToAllPlayers(Player** first, const int numPlayers, nlohmann::json& data);
	void SendDataToAllPlayersInSession(GameSession* session, nlohmann::json& data);

private:
	char m_Port[5];

	SOCKET m_ServerSocket;

	bool PlayerIdCheck(TGATPLAYERID playerId) override;
	bool PlayerIdCheck(TGATPLAYERID playerId, GameSession* session);
};

