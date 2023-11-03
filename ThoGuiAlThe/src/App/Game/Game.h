#pragma once
#include "Board/Board.h"
#include "Player/Player.h"

class Game
{
public:
	Game();
	~Game();

	void Init();
	void HandleEvents(sf::Event* event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

private:
	Board* m_Board;

	void m_InitBoard();
	void m_HandleBoardEvents(sf::Event* event);
	void m_UpdateBoard(const float& dt);
	void m_RenderBoard(sf::RenderTarget* target);

private:
	std::vector<Player*> m_Players;

	void m_InitPlayers();
	void m_HandlePlayersEvents(sf::Event* event);
	void m_UpdatePlayers(const float& dt);
	void m_RenderPlayers(sf::RenderTarget* target);

private:
	unsigned int m_Turn = 0;
};