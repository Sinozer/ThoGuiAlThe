#pragma once

#include "Cell/Cell.h"

class Board final : public sf::RectangleShape
{
public:
	explicit Board(const std::vector<Player*>& players);
	~Board() override;

	void Init();
	void HandleEvents(sf::Event* event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

private:
	std::vector<std::vector<Cell*>> m_Cells;

	void InitCells();
	void HandleCellsEvents(sf::Event* event);

	// Returns true if the clicked cell is already occupied
	bool HandleMouseLeftClick(sf::Event* event);
	void UpdateCells(const float& dt);
	void RenderCells(sf::RenderTarget* target);

private:
	unsigned int m_Turn;
	std::vector<Player*> m_Players;
};