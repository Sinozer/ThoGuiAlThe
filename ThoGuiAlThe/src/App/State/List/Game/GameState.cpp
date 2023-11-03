#include "stdafx.h"
#include "GameState.h"

GameState::GameState()
{
}
GameState::~GameState()
{
	delete m_Game;
}

void GameState::m_InitBackground()
{
}
void GameState::m_InitUI()
{
	auto* title = m_UIManager.AddText("TITLE", "TICTACTOE");
	title->setCharacterSize(100);
	title->setFillColor(sf::Color(38, 70, 83));
	title->setOutlineColor(sf::Color::White);
	title->setOutlineThickness(4.f);
	title->setPosition(
		(WINDOW_SCREEN_WIDTH - title->getGlobalBounds().width) / 2,
		100.f
	);

}
void GameState::m_InitGame()
{
	m_Game = new Game();
	m_Game->Init();
}
void GameState::Init()
{
	m_ClearColor = sf::Color(233, 196, 106);
	m_InitBackground();
	m_InitUI();
	m_InitGame();
}

void GameState::m_HandleUiEvents(sf::Event& event)
{
	m_UIManager.HandleEvents(event);
}
void GameState::m_HandleGameEvents(sf::Event& event)
{
	m_Game->HandleEvents(&event);
}
void GameState::HandleEvents(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		if (event.key.code == sf::Keyboard::Escape)
		{
			StateManager::GetInstance()->RemoveState();
			return;
		}
	m_HandleUiEvents(event);
	m_HandleGameEvents(event);
}

void GameState::m_UpdateUI(const float& dt)
{
	m_UIManager.Update(dt);
}
void GameState::m_UpdateGame(const float& dt)
{
	m_Game->Update(dt);
}
void GameState::Update(const float& dt)
{
	m_UpdateUI(dt);
	m_UpdateGame(dt);
}

void GameState::m_RenderUI(sf::RenderTarget* target)
{
	m_UIManager.Render(target);
}
void GameState::m_RenderGame(sf::RenderTarget* target)
{
	m_Game->Render(target);
}
void GameState::Render(sf::RenderTarget* target)
{
	if (target == nullptr)
		return;

	target->draw(m_Background);

	m_RenderUI(target);
	m_RenderGame(target);
}

void GameState::End()
{
}