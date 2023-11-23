#pragma once
#include "Session/GameSession.h"
class GameManager 
{
public:
	GameManager() = default;
	~GameManager();

	GameSession* CreateGameSession(Player* p1);
	void AddPlayerToGameSession(Player* p2, uint32_t id = -1);
	void UpdateGameSession(uint32_t id, uint32_t playerId, int x, int y, nlohmann::json& returnJson);

	GameSession* GetWaitingSessionById(uint32_t id) const;
	GameSession* GetFirstWaitingSession() const { return m_WaitingSessions.begin()->second;}

	GameSession* GetActiveSessionById(uint32_t id) const;

	void OnServerClose();

private:
	const int MAGIC_NUMBER = 7817;

	std::unordered_map<uint32_t, GameSession*> m_WaitingSessions;
	std::unordered_map<uint32_t, GameSession*> m_ActiveSessions;

	void EndGameSession(uint32_t id);
};