#pragma once

class Player;

class GameSession
{
public:
	static constexpr int MAX_PLAYERS_PER_GAME = 2;
private:
	static constexpr int BOARD_SIZE = 3;

public:
	GameSession();
	~GameSession();

	uint32_t GetId() const { return m_Id; }

	// Returns ID
	void Init(Player* p1, int magic);
	void AddPlayer(Player* p2);
	void Update(uint32_t playerId, int x, int y, nlohmann::json& returnJson);

	bool IsEnded() const { return m_IsEnded; }
	std::array<Player*, MAX_PLAYERS_PER_GAME>& GetPlayers() { return m_Players; }

private:
	std::array<Player*, MAX_PLAYERS_PER_GAME> m_Players;
	std::unordered_set<Player*> m_Spectators;

	uint32_t m_Board[BOARD_SIZE][BOARD_SIZE];
	unsigned char m_Turn;
	bool m_IsEnded;

	// Id = player1.id * magic number (prime number)
	uint32_t m_Id;

	bool IsFull() const;
	bool IsWin() const;
};