#pragma once
#include "Session/GameSession.h"
class GameManager 
{
public:
	GameManager();
	~GameManager();

	GameSession* CreateGameSession(Player* p1);
	void RemoveGameSession(uint32_t id);
	void AddPlayerToGameSession(Player* p2, uint32_t id = -1);
	void UpdateGameSession(uint32_t id, uint32_t playerId, int x, int y, nlohmann::json& returnJson);

	GameSession* GetWaitingSessionById(uint32_t id) const;
	GameSession* GetFirstWaitingSession() const;

	GameSession* GetActiveSessionById(uint32_t id) const;
	std::unordered_map<uint32_t, GameSession*> GetActiveSessions() const { return m_ActiveSessions; }

	CriticalSectionScope GetActiveSessionsCSScope() { return { m_ActiveSessionsLock }; }

	void OnServerClose();

private:
	const int MAGIC_NUMBER;

	std::unordered_map<uint32_t, GameSession*> m_WaitingSessions;
	std::unordered_map<uint32_t, GameSession*> m_ActiveSessions;

	CRITICAL_SECTION m_ActiveSessionsLock;

	void EndGameSession(uint32_t id);
};