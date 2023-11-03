#pragma once

#include "Cell/Cell.h"

class Board : public sf::RectangleShape
{
public:
	Board(unsigned int& turn, std::vector<Player*>& players);
	~Board();

	void Init();
	void HandleEvents(sf::Event* event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

private:
	std::vector<std::vector<Cell*>> m_Cells;

	void m_InitCells();
	void m_HandleCellsEvents(sf::Event* event);
	void m_UpdateCells(const float& dt);
	void m_RenderCells(sf::RenderTarget* target);

private:
	unsigned int& m_Turn;
	std::vector<Player*>& m_Players;
};