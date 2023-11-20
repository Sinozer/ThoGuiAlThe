#include "tgatLib.h"
#include "Player/Player.h"

#include "GameNetworkManager.h"

#include "Player/PlayerManager.h"
#include "Network/Http/HttpManager.h"
#include "Network/Server.h"
#include "Exceptions/TgatException.h"

GameNetworkManager::GameNetworkManager() : TgatNetworkHelper(), m_ServerSocket(INVALID_SOCKET), m_Port("6969")
{
}

GameNetworkManager::~GameNetworkManager()
{
    closesocket(m_ServerSocket);
}

void GameNetworkManager::Init()
{
}

void GameNetworkManager::SendDataToPlayer(const Player& player, nlohmann::json& data)
{
	try
	{
		TgatNetworkHelper::Message msg;
		std::string strData = data.dump();
		CreateMessage(HEADER_ID, player.GetId(), strData, msg);
		Send(player.GetSocket(), msg);
	}
	catch (const TgatException& e)
	{
		LOG(e.what());
	}
}

void GameNetworkManager::SendDataToAllPlayers(std::unordered_set<Player>& players, nlohmann::json& data)
{
	for (const auto& player : players)
	{
		SendDataToPlayer(player, data);
	}
}

bool GameNetworkManager::PlayerIdCheck(TGATPLAYERID playerId)
{
	// Check if the player id is in the list of connected players
	const auto& players = I(Server).GetPlayerManager()->GetPlayers();
	auto it = std::find_if(players.begin(), players.end(), [playerId](const Player& player)
		{
			return player.GetId() == playerId;
		});

	return it != players.end();
}
