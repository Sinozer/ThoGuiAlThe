#pragma once

#include "Player/Player.h"

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

public:
	int AddPlayer(Player& player);
	void RemovePlayer(Player& player);
	void RemovePlayer(SOCKET socket);
	std::unordered_set<Player>& GetPlayers() { return m_Players; }

private:
	std::unordered_set<Player> m_Players;
};

