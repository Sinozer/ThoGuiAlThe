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
	void RemovePlayer(Player* player);
	void Update(uint32_t playerId, int x, int y, nlohmann::json& returnJson);

	bool IsEnded() const { return m_IsEnded; }
	std::unordered_map<uint32_t, Player*>& GetPlayers() { return m_Players; }

private:
	std::unordered_map<uint32_t, Player*> m_Players;
	std::unordered_set<Player*> m_Spectators;

	uint32_t m_Board[BOARD_SIZE][BOARD_SIZE];
	unsigned char m_Turn;
	bool m_IsEnded;

	// Id = player1.id * magic number (prime number)
	uint32_t m_Id;

	bool IsFull() const;
	bool IsWin() const;

public:
	const int GetReplayCount() const { return m_ReplayerIds.size(); }
	void AddReplayer(TGATPLAYERID id) { m_ReplayerIds.push_back(id); }
	void Replay();
	bool IsReplayer(TGATPLAYERID id) const { return std::find(m_ReplayerIds.begin(), m_ReplayerIds.end(), id) != m_ReplayerIds.end(); }
private:
	std::vector<TGATPLAYERID> m_ReplayerIds;
};