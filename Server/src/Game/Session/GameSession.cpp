#include "Player/Player.h"
#include "GameSession.h"

GameSession::GameSession() : m_Board(), m_Players(), m_Turn(0), m_Id(0), m_IsEnded(false)
{
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			m_Board[i][j] = 0;
}

GameSession::~GameSession()
{
	m_Spectators.clear();
	m_Players.clear();
}

void GameSession::Init(Player* p1, int magic)
{
	m_Players.emplace(std::make_pair(0, p1));

	m_Id = p1->GetId() * magic;
}

void GameSession::AddPlayer(Player* p2)
{
	m_Players.emplace(std::make_pair(1, p2));
}

void GameSession::Update(uint32_t playerId, int x, int y, nlohmann::json& returnJson)
{
	uint32_t id = m_Players[m_Turn % 2]->GetId();
	if (id != playerId)
	{
		returnJson =
		{
			{JSON_EVENT_TYPE, TgatServerMessage::BAD},
			{JSON_ERROR, "Not your turn"}
		};
		return;
	}

	if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
	{
		returnJson = 
		{
			{JSON_EVENT_TYPE, TgatServerMessage::BAD},
			{JSON_ERROR, "Invalid position"}
		};
		return;
	}

	if (m_Board[x][y] != 0)
	{
		returnJson =
		{
			{JSON_EVENT_TYPE, TgatServerMessage::BAD},
			{JSON_ERROR, "Position already taken"}
		};
		return;
	}

	m_Board[x][y] = id;
	m_Turn++;

	if (IsWin() == true)
	{
		returnJson =
		{
			{JSON_EVENT_TYPE, TgatServerMessage::GAME_END},
			{JSON_PLAYER_ID, id},
			{JSON_PLAYER_MOVE, PLAYER_MOVE(x, y)}
		};
		m_IsEnded = true;
		return;
	}
	else if (IsFull() == true)
	{
		returnJson =
		{
			{JSON_EVENT_TYPE, TgatServerMessage::GAME_END},
			{JSON_PLAYER_ID, 0},
			{JSON_PLAYER_MOVE, PLAYER_MOVE(x, y)}
		};
		m_IsEnded = true;
		return;
	}

	returnJson =
	{
		{JSON_EVENT_TYPE, TgatServerMessage::PLAYER_INPUT},
		{JSON_PLAYER_ID, id},
		{JSON_PLAYER_MOVE, PLAYER_MOVE(x, y)}
	};
}

bool GameSession::IsFull() const
{
	return m_Turn == BOARD_SIZE * BOARD_SIZE;
}

bool GameSession::IsWin() const
{
	// Check rows and columns
	for (int i = 0; i < 3; i++) {
		if ((m_Board[i][0] == m_Board[i][1] && m_Board[i][1] == m_Board[i][2] && m_Board[i][0] != 0) ||
			(m_Board[0][i] == m_Board[1][i] && m_Board[1][i] == m_Board[2][i] && m_Board[0][i] != 0)) {
			return true;
		}
	}

	// Check diagonals
	if ((m_Board[0][0] == m_Board[1][1] && m_Board[1][1] == m_Board[2][2] && m_Board[0][0] != 0) ||
		(m_Board[0][2] == m_Board[1][1] && m_Board[1][1] == m_Board[2][0] && m_Board[0][2] != 0)) {
		return true;
	}

	return false;
}

void GameSession::Replay()
{
	m_Board[0][0] = 0;
	m_Board[0][1] = 0;
	m_Board[0][2] = 0;
	m_Board[1][0] = 0;
	m_Board[1][1] = 0;
	m_Board[1][2] = 0;
	m_Board[2][0] = 0;
	m_Board[2][1] = 0;
	m_Board[2][2] = 0;

	m_Turn = 0;
	m_IsEnded = false;

	m_ReplayerIds.clear();
}