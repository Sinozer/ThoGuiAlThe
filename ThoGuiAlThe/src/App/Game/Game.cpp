#include "Game.h"

Game::Game()
{
	m_Board = nullptr;
}

Game::~Game()
{
	DELPTR(m_Board);

	for (auto& player : m_Players)
	{
		DELPTR(player);
	}
}

void Game::InitBoard()
{
	m_Board = new Board(m_Players);
	m_Board->Init();
}
void Game::InitPlayers()
{
	m_Players.push_back(new Player(sf::Color(231, 111, 81)));
	m_Players.push_back(new Player(sf::Color(42, 157, 143)));
}
void Game::Init()
{
	InitPlayers();
	InitBoard();
}

void Game::HandleBoardEvents(sf::Event* event)
{
	m_Board->HandleEvents(event);
}

void Game::HandlePlayersEvents(sf::Event* event)
{
	for (auto& player : m_Players)
		player->HandleEvents(event);
}

void Game::HandleEvents(sf::Event* event)
{
	HandleBoardEvents(event);
	HandlePlayersEvents(event);
}

void Game::UpdateBoard(const float& dt)
{
	m_Board->Update(dt);
}
void Game::UpdatePlayers(const float& dt)
{
	for (auto& player : m_Players)
		player->Update(dt);
}
void Game::Update(const float& dt)
{
	UpdateBoard(dt);
	UpdatePlayers(dt);
}

void Game::RenderBoard(sf::RenderTarget* target)
{
	m_Board->Render(target);
}
void Game::RenderPlayers(sf::RenderTarget* target)
{
	for (const auto& player : m_Players)
		player->Render(target);
}
void Game::Render(sf::RenderTarget* target)
{
	RenderBoard(target);
	RenderPlayers(target);
}