#include "stdafx.h"
#include "HomeState.h"

#include "App/State/List/Game/GameState.h"

HomeState::HomeState()
{
}


void HomeState::m_InitBackground()
{
}
void HomeState::m_InitUI()
{
	m_UIManager.AddText("TITLE");
}
void HomeState::Init()
{
	m_InitBackground();
	m_InitUI();
}

void HomeState::End()
{
}

void HomeState::HandleEvents(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		if (event.key.code == sf::Keyboard::Enter)
			StateManager::GetInstance()->AddState(new GameState());
}

void HomeState::m_UpdateUI(const float& dt)
{
	m_UIManager.Update(dt);
}
void HomeState::Update(const float& dt)
{
	m_UpdateUI(dt);
}

void HomeState::m_RenderUI(sf::RenderTarget* target)
{
	m_UIManager.Render(target);
}
void HomeState::Render(sf::RenderTarget* target)
{
	if (target == nullptr)
		return;

	target->draw(m_Background);

	m_RenderUI(target);
}