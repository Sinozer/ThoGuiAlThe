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
	for (Player* player : m_Players | std::views::values)
	{
		closesocket(player->GetSocket());
	}
}

int PlayerManager::AddPlayer(Player* player)
{
	m_Players.emplace(std::make_pair(player->GetId(), player));
	LOG("New number of player : " << m_Players.size());
	return (int)m_Players.size();
}

void PlayerManager::RemovePlayer(Player* player)
{
	nlohmann::json jsonData =
	{
		{"eventType", TgatServerMessage::PLAYER_DISCONNECT},
		{"message", "bye bye"}
	};

	I(Server).GetGameNetworkManager()->SendDataToPlayer(player, jsonData);

	if (int r = closesocket(player->GetSocket()) != 0)
	{
		LOG("closesocket failed with error: " << r);
		return;
	}
	else
		LOG("closesocket success");

	m_Players.erase(player->GetId());
	LOG("Number of clients: " << m_Players.size());
}

void PlayerManager::RemovePlayer(SOCKET socket)
{
	auto it = std::find_if(m_Players.begin(), m_Players.end(), [socket](std::pair<uint32_t, Player*> player)
		{
			return player.second->GetSocket() == socket;
		});

	if (it != m_Players.end())
	{
		RemovePlayer(it->second);
	}
	else
	{
		LOG("Player not found");
	}
}

Player* PlayerManager::GetPlayerById(uint32_t index)
{
	auto it = m_Players.find(index);
	if (it != m_Players.end())
	{
		return it->second;
	}
	LOG("Player not found");
	return nullptr;
}
