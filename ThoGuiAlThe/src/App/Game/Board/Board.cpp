#include "stdafx.h"
#include "Board.h"

Board::Board(unsigned int& turn, std::vector<Player*>& players)
	: m_Turn(turn)
	, m_Players(players)
{
}

Board::~Board()
{
	for (auto& row : m_Cells)
	{
		for (auto& cell : row)
		{
			delete cell;
		}
	}
}

void Board::m_InitCells()
{
	for (int i = 0; i < 3; i++)
	{
		std::vector<Cell*> row;
		for (int j = 0; j < 3; j++)
		{
			Cell* cell = new Cell(sf::Vector2u(i, j));
			cell->Init();
			row.push_back(cell);
		}
		m_Cells.push_back(row);
	}
}
void Board::Init()
{
	setSize(sf::Vector2f(CELL_SIZE_X * BOARD_SIZE_X, CELL_SIZE_Y * BOARD_SIZE_Y));
	setPosition(
		(WINDOW_SCREEN_WIDTH - getGlobalBounds().width) / 2,
		(WINDOW_SCREEN_HEIGHT - getGlobalBounds().height) / 2
	);
	setOutlineThickness(2.f);
	setOutlineColor(sf::Color::Black);

	m_InitCells();
}

void Board::m_HandleCellsEvents(sf::Event* event)
{
	for (auto& row : m_Cells)
	{
		for (auto& cell : row)
		{
			cell->HandleEvents(event);
		}
	}
}
void Board::HandleEvents(sf::Event* event)
{
	if (event->type == sf::Event::MouseButtonPressed)
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			if (getGlobalBounds().contains(sf::Vector2f(event->mouseButton.x, event->mouseButton.y)))
			{
				sf::Vector2u cellPosition(
					(event->mouseButton.x - getPosition().x) / CELL_SIZE_X,
					(event->mouseButton.y - getPosition().y) / CELL_SIZE_Y
				);
				m_Cells[cellPosition.x][cellPosition.y]->HandleEvents(event);
				m_Cells[cellPosition.x][cellPosition.y]->SetPlayer(m_Players[m_Turn % 2]);
				m_Turn++;
			}
		}
	}
	//m_HandleCellsEvents(event);
}

void Board::m_UpdateCells(const float& dt)
{
	for (auto& row : m_Cells)
	{
		for (auto& cell : row)
		{
			cell->Update(dt);
		}
	}
}
void Board::Update(const float& dt)
{
	m_UpdateCells(dt);
}

void Board::m_RenderCells(sf::RenderTarget* target)
{
	for (auto& row : m_Cells)
	{
		for (auto& cell : row)
		{
			cell->Render(target);
		}
	}
}
void Board::Render(sf::RenderTarget* target)
{
	target->draw(*this);

	m_RenderCells(target);
}