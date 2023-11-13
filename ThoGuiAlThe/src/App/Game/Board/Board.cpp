#include "stdafx.h"
#include "Board.h"

Board::Board(const std::vector<Player*>& players) : m_Turn(0), m_Players(players)
{
}

Board::~Board()
{
    for (auto& row : m_Cells)
    {
        for (const auto& cell : row)
        {
            delete cell;
        }
        row.clear();
    }
    m_Cells.clear();

    for (const auto& player : m_Players)
    {
        delete player;
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

        m_Cells[cellPosition.x][cellPosition.y]->SetPlayer(m_Players[m_Turn % 2]);
        m_Turn++;
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
