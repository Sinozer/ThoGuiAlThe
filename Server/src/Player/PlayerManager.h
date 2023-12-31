#pragma once

#include "Player/Player.h"

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

public:
	int AddPlayer(Player* player);
	void RemovePlayer(Player* player);
	void RemovePlayer(SOCKET socket);

	std::unordered_map<uint32_t, Player*>& GetPlayers() { return m_Players; }
	Player* GetPlayerById(uint32_t index);

private:
	std::unordered_map<uint32_t, Player*> m_Players;
};

