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
	auto* title = m_UIManager.AddText("TITLE", "THOGUIALTHE");
	title->setCharacterSize(100);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2, 100.f);

	auto* start = m_UIManager.AddButton("START", "PLAY", [this]() { StateManager::GetInstance()->AddState(new GameState()); });
	start->setCharacterSize(50);
	start->setOutlineColor(sf::Color::Black);
	start->setOutlineThickness(2.f);
	start->setPosition(WINDOW_SCREEN_WIDTH / 2 - start->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - start->getGlobalBounds().height - 300.f);

	auto* exit = m_UIManager.AddButton("EXIT", "EXIT", [this]() { StateManager::GetInstance()->RemoveAllStates(); });
	exit->setOutlineColor(sf::Color::Black);
	exit->setOutlineThickness(2.f);
	exit->setPosition(WINDOW_SCREEN_WIDTH / 2 - exit->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - exit->getGlobalBounds().height - 50.f);
}
void HomeState::Init()
{
	m_InitBackground();
	m_InitUI();
}

void HomeState::End()
{
}

void HomeState::m_HandleUiEvents(sf::Event& event)
{
	m_UIManager.HandleEvents(event);
}
void HomeState::HandleEvents(sf::Event& event)
{
	m_HandleUiEvents(event);
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