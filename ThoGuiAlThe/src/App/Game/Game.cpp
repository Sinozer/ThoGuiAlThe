#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
	delete m_Board;

	for (auto& player : m_Players)
	{
		delete player;
	}
}

void Game::m_InitBoard()
{
	m_Board = new Board(m_Turn, m_Players);
	m_Board->Init();
}
void Game::m_InitPlayers()
{
	m_Players.push_back(new Player(sf::Color(231, 111, 81)));
	m_Players.push_back(new Player(sf::Color(42, 157, 143)));
}
void Game::Init()
{
	m_InitBoard();
	m_InitPlayers();
}

void Game::m_HandleBoardEvents(sf::Event* event)
{
	m_Board->HandleEvents(event);
}
void Game::m_HandlePlayersEvents(sf::Event* event)
{
	for (auto& player : m_Players)
		player->HandleEvents(event);
}
void Game::HandleEvents(sf::Event* event)
{
	m_HandleBoardEvents(event);
	m_HandlePlayersEvents(event);
}

void Game::m_UpdateBoard(const float& dt)
{
	m_Board->Update(dt);
}
void Game::m_UpdatePlayers(const float& dt)
{
	for (auto& player : m_Players)
		player->Update(dt);
}
void Game::Update(const float& dt)
{
	m_UpdateBoard(dt);
	m_UpdatePlayers(dt);
}

void Game::m_RenderBoard(sf::RenderTarget* target)
{
	m_Board->Render(target);
}
void Game::m_RenderPlayers(sf::RenderTarget* target)
{
	for (auto& player : m_Players)
		player->Render(target);
}
void Game::Render(sf::RenderTarget* target)
{
	m_RenderBoard(target);
	m_RenderPlayers(target);
}