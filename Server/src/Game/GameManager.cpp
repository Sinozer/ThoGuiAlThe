#include "stdafx.h"
#include "GameManager.h"

GameManager::~GameManager()
{

}

GameSession* GameManager::CreateGameSession(Player* p1)
{
	GameSession* session = new GameSession();
	session->Init(p1, MAGIC_NUMBER);
	m_WaitingSessions.insert(std::make_pair(session->GetId(), session));
	return session;
}

void GameManager::AddPlayerToGameSession(Player* p2, uint32_t id)
{
	GameSession* session = nullptr;

	if (id == -1)
	{
		if (m_WaitingSessions.empty())
		{
			CreateGameSession(p2);
			return;
		}

		session = m_WaitingSessions.begin()->second;
	}
	else
	{
		auto it = m_WaitingSessions.find(id);
		if (it == m_WaitingSessions.end())
		{
			//TODO send invalid session id
			LOG("Invalid session id : GameManager::AddPlayerToGameSession()");
			return;
		}

		session = it->second;
	}

	session->AddPlayer(p2);
	m_ActiveSessions.insert(std::make_pair(session->GetId(), session));
	m_WaitingSessions.erase(session->GetId());
}

void GameManager::EndGameSession(uint32_t id)
{
	auto it = m_ActiveSessions.find(id);
	if (it == m_ActiveSessions.end())
	{
		//TODO send invalid session id
		LOG("Invalid session id : GameManager::EndGameSession()");
		return;
	}

	DELPTR(it->second);
	m_ActiveSessions.erase(it);
}

void GameManager::UpdateGameSession(uint32_t id, uint32_t playerId, int x, int y, nlohmann::json& returnJson)
{
	auto it = m_ActiveSessions.find(id);
	if (it == m_ActiveSessions.end())
	{
		//TODO send invalid session id
		LOG("Invalid session id : GameManager::UpdateGameSession()");
		return;
	}

	it->second->Update(playerId, x, y, returnJson);

	if (it->second->IsEnded()) EndGameSession(id);
}

GameSession* GameManager::GetWaitingSessionById(uint32_t id) const
{
	auto it = m_WaitingSessions.find(id);
	if (it == m_WaitingSessions.end())
	{
		LOG("Invalid session id : GameManager::GetWaitingSessionById()");
		return nullptr;
	}

	return it->second;
}

GameSession* GameManager::GetActiveSessionById(uint32_t id) const
{
	auto it = m_ActiveSessions.find(id);
	if (it == m_ActiveSessions.end())
	{
		LOG("Invalid session id : GameManager::GetActiveSessionById()");
		return nullptr;
	}

	return it->second;
}

void GameManager::OnServerClose()
{
	for (auto& session : m_WaitingSessions | std::views::values)
	{
		DELPTR(session);
	}
	m_WaitingSessions.clear();

	for (auto& session : m_ActiveSessions | std::views::values)
	{
		DELPTR(session);
	}
	m_ActiveSessions.clear();
}
