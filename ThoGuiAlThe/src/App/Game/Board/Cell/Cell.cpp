#include "stdafx.h"
#include "Cell.h"

#include "Constants.h"

Cell::Cell(sf::Vector2u position)
	: m_Player(nullptr)
	, m_Position(position)
{
}

Cell::~Cell()
{
}

void Cell::Init()
{
	setSize(sf::Vector2f(CELL_SIZE_X, CELL_SIZE_Y));
	setPosition(
		m_Position.x * CELL_SIZE_X + WINDOW_SCREEN_WIDTH / 2 - getGlobalBounds().width / 2 * BOARD_SIZE_X,
		m_Position.y * CELL_SIZE_Y + WINDOW_SCREEN_HEIGHT / 2 - getGlobalBounds().height / 2 * BOARD_SIZE_Y
	);
	setFillColor(sf::Color::Transparent);
	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Black);
}

void Cell::HandleEvents(sf::Event* event)
{
	std::cout << "Cell clicked: " << m_Position.x << " " << m_Position.y << std::endl;
}

void Cell::Update(const float& dt)
{
}

void Cell::Render(sf::RenderTarget* target)
{
	target->draw(*this);

	if (m_Player != nullptr)
	{
		sf::RectangleShape player(sf::Vector2f(PLAYER_SIZE_X, PLAYER_SIZE_Y));
		player.setFillColor(m_Player->GetColor());
		player.setPosition(
			m_Position.x * CELL_SIZE_X + PLAYER_MARGIN_X * 1.5f + WINDOW_SCREEN_WIDTH / 2 - getGlobalBounds().width / 2 * BOARD_SIZE_X,
			m_Position.y * CELL_SIZE_Y + PLAYER_MARGIN_Y * 1.5f + WINDOW_SCREEN_HEIGHT / 2 - getGlobalBounds().height / 2 * BOARD_SIZE_Y
		);

		target->draw(player);
	}
}