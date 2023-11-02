#include "stdafx.h"
#include "GameState.h"

GameState::GameState()
{
}


void GameState::m_InitBackground()
{
}
void GameState::m_InitUI()
{
}
void GameState::Init()
{
	m_ClearColor = sf::Color(233, 196, 106);
	m_InitBackground();
}

void GameState::End()
{
}

void GameState::m_HandleUiEvents(sf::Event& event)
{
	m_UIManager.HandleEvents(event);
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
}

void GameState::m_UpdateUI(const float& dt)
{
	m_UIManager.Update(dt);
}
void GameState::Update(const float& dt)
{
	m_UpdateUI(dt);
}

void GameState::m_RenderUI(sf::RenderTarget* target)
{
	m_UIManager.Render(target);
}
void GameState::Render(sf::RenderTarget* target)
{
	if (target == nullptr)
		return;

	target->draw(m_Background);

	m_RenderUI(target);
}