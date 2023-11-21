#include "tgatLib.h"
#include "Player/Player.h"

#include "PlayerManager.h"

#include "Network/Server.h"
#include "Network/GameNetwork/GameNetworkManager.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
	for (Player player : m_Players)
	{
		closesocket(player.GetSocket());
	}
}

int PlayerManager::AddPlayer(Player& player)
{
	m_Players.emplace(std::move(player));
	LOG("New number of player : " << m_Players.size());
	return (int)m_Players.size();
}

void PlayerManager::RemovePlayer(Player& player)
{
	nlohmann::json jsonData =
	{
		{"eventType", TgatServerMessage::PLAYER_DISCONNECT},
		{"message", "bye bye"}
	};

	I(Server).GetGameNetworkManager()->SendDataToPlayer(player, jsonData);

	if (int r = closesocket(player.GetSocket()) != 0)
	{
		LOG("closesocket failed with error: " << r);
		return;
	}
	else
		LOG("closesocket success");

	m_Players.erase(player);
	LOG("Number of clients: " << m_Players.size());
}

void PlayerManager::RemovePlayer(SOCKET socket)
{
	auto it = std::find_if(m_Players.begin(), m_Players.end(), [socket](const Player& player)
		{
			return player.GetSocket() == socket;
		});

	if (it != m_Players.end())
	{
		Player player = *it;
		RemovePlayer(player);
	}
	else
	{
		LOG("Player not found");
	}
}
