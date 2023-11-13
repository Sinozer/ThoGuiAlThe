#include "HomeState.h"

#include "App/State/List/Game/GameState.h"

HomeState::HomeState()
= default;


void HomeState::InitBackground()
{
}
void HomeState::InitUi()
{
	auto* title = m_UiManager.AddText("TITLE", "THOGUIALTHE");
	title->setCharacterSize(100);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2, 100.f);

	auto* start = m_UiManager.AddButton("START", "PLAY", [this]() { StateManager::GetInstance()->AddState(new GameState()); });
	start->setCharacterSize(50);
	start->setOutlineColor(sf::Color::Black);
	start->setOutlineThickness(2.f);
	start->setPosition(WINDOW_SCREEN_WIDTH / 2 - start->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - start->getGlobalBounds().height - 300.f);

	auto* exit = m_UiManager.AddButton("EXIT", "EXIT", [this]() { StateManager::GetInstance()->RemoveAllStates(); });
	exit->setOutlineColor(sf::Color::Black);
	exit->setOutlineThickness(2.f);
	exit->setPosition(WINDOW_SCREEN_WIDTH / 2 - exit->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - exit->getGlobalBounds().height - 50.f);
}
void HomeState::Init()
{
	InitBackground();
	InitUi();
}

void HomeState::End()
{
}

void HomeState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void HomeState::HandleEvents(sf::Event& event)
{
	HandleUiEvents(event);
}

void HomeState::UpdateUi(const float& dt)
{
	m_UiManager.Update(dt);
}
void HomeState::Update(const float& dt)
{
	UpdateUi(dt);
}

void HomeState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void HomeState::Render(sf::RenderTarget* target)
{
	if (target == nullptr)
		return;

	target->draw(m_Background);

	RenderUi(target);
}