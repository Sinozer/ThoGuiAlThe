#include "Board.h"
#include "App/State/List/Result/ResultState.h"

#include "App/Network/NetworkManager.h"

Board::Board(const std::vector<Player*>& players) : m_Turn(0), m_Players(players)
{
}

Board::~Board()
{
	for (auto& row : m_Cells)
	{
		for (auto& cell : row)
		{
			if (cell != nullptr)
			{
				delete cell;
				cell = nullptr;
			}
		}
		row.clear();
	}
	m_Cells.clear();

	for (auto& player : m_Players)
	{
		player = nullptr;
	}
	m_Players.clear();
}

void Board::InitCells()
{
	for (int i = 0; i < 3; i++)
	{
		std::vector<Cell*> row;
		for (int j = 0; j < 3; j++)
		{
			auto* cell = new Cell(sf::Vector2u(i, j));
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
		(WINDOW_SCREEN_HEIGHT - getGlobalBounds().height) / 1.5f
	);
	setOutlineThickness(2.f);
	setOutlineColor(sf::Color::Black);

	InitCells();
}

void Board::HandleCellsEvents(sf::Event* event)
{
	for (auto& row : m_Cells)
	{
		for (const auto& cell : row)
		{
			cell->HandleEvents(event);
		}
	}
}

bool Board::HandleMouseLeftClick(sf::Event* event)
{
	if (getGlobalBounds().contains(sf::Vector2f(static_cast<float>(event->mouseButton.x), static_cast<float>(event->mouseButton.y))))
	{
		const sf::Vector2u cellPosition(
			static_cast<unsigned>((static_cast<float>(event->mouseButton.x) - getPosition().x) / CELL_SIZE_X),
			static_cast<unsigned>((static_cast<float>(event->mouseButton.y) - getPosition().y) / CELL_SIZE_Y)
		);
		m_Cells[cellPosition.x][cellPosition.y]->HandleEvents(event);

		if (m_Cells[cellPosition.x][cellPosition.y]->GetPlayer() != nullptr)
			return true;

		/*m_Cells[cellPosition.x][cellPosition.y]->SetPlayer(m_Players[m_Turn % 2]);
		m_Turn++;*/
	}
	return false;
}

void Board::HandleEvents(sf::Event* event)
{
	if (event->type == sf::Event::MouseButtonPressed)
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			const bool cellNotEmpty = HandleMouseLeftClick(event);
			if (cellNotEmpty) return;
		}
	}
	//m_HandleCellsEvents(event);
}

void Board::UpdateCells(const float& dt)
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
	UpdateCells(dt);

	nlohmann::json data;

	if (I(NetworkManager).ReceiveData(TgatServerMessage::PLAYER_INPUT, data) == false)
		return;

	Player* p;

	int playerId = data[JSON_PLAYER_ID];

	if (playerId == I(NetworkManager).GetPlayerId())
		p = m_Players[0];
	else
		p = m_Players[1];

	int x = PLAYER_MOVE_ARG_X(data[JSON_PLAYER_MOVE]);
	int y = PLAYER_MOVE_ARG_Y(data[JSON_PLAYER_MOVE]);

	m_Cells[x][y]->SetPlayer(p);
}

void Board::RenderCells(sf::RenderTarget* target)
{
	for (auto& row : m_Cells)
	{
		for (const auto& cell : row)
		{
			cell->Render(target);
		}
	}
}

void Board::Render(sf::RenderTarget* target)
{
	target->draw(*this);

	RenderCells(target);
}

bool Board::IsFull() const
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (m_Cells[i][j]->GetPlayer() == nullptr)
				return false;
	return true;
}

bool Board::IsWin() const
{
	// Check rows and columns
	for (int i = 0; i < 3; i++) {
		if ((m_Cells[i][0]->GetPlayer() == m_Cells[i][1]->GetPlayer() && m_Cells[i][1]->GetPlayer() == m_Cells[i][2]->GetPlayer() && m_Cells[i][0]->GetPlayer() != nullptr) ||
			(m_Cells[0][i]->GetPlayer() == m_Cells[1][i]->GetPlayer() && m_Cells[1][i]->GetPlayer() == m_Cells[2][i]->GetPlayer() && m_Cells[0][i]->GetPlayer() != nullptr)) {
			return true;
		}
	}

	// Check diagonals
	if ((m_Cells[0][0]->GetPlayer() == m_Cells[1][1]->GetPlayer() && m_Cells[1][1]->GetPlayer() == m_Cells[2][2]->GetPlayer() && m_Cells[0][0]->GetPlayer() != nullptr) ||
		(m_Cells[0][2]->GetPlayer() == m_Cells[1][1]->GetPlayer() && m_Cells[1][1]->GetPlayer() == m_Cells[2][0]->GetPlayer() && m_Cells[0][2]->GetPlayer() != nullptr)) {
		return true;
	}

	return false;
}
