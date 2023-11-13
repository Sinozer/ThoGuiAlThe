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
	void InitBoard();
	void HandleBoardEvents(sf::Event* event);
	void UpdateBoard(const float& dt);
	void RenderBoard(sf::RenderTarget* target);
	
	void InitPlayers();
	void HandlePlayersEvents(sf::Event* event);
	void UpdatePlayers(const float& dt);
	void RenderPlayers(sf::RenderTarget* target);

private:
	std::vector<Player*> m_Players;
	Board* m_Board;
};