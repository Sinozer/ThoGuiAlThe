#include "SelectState.h"

SelectState::SelectState()
{
}

void SelectState::InitBackground()
{
}
void SelectState::InitUi()
{
	auto* title = m_UiManager.AddText("TITLE", "TICTACTOE");
	title->setCharacterSize(100);
	title->setFillColor(sf::Color::White);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(
		(WINDOW_SCREEN_WIDTH - title->getGlobalBounds().width) / 2,
		100.f
	);

	auto* create = m_UiManager.AddButton("CREATE", "CREATE", [this]() { /*StateManager::GetInstance()->AddState(new CreatingState());*/ });
	create->setCharacterSize(50);
	create->setOutlineColor(sf::Color::Black);
	create->setOutlineThickness(2.f);
	create->setPosition(WINDOW_SCREEN_WIDTH / 2 - create->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - create->getGlobalBounds().height - 350.f);

	auto* join = m_UiManager.AddButton("JOIN", "JOIN", [this]() { /*StateManager::GetInstance()->AddState(new JoiningState());*/ });
	join->setCharacterSize(50);
	join->setOutlineColor(sf::Color::Black);
	join->setOutlineThickness(2.f);
	join->setPosition(WINDOW_SCREEN_WIDTH / 2 - join->getGlobalBounds().width / 2, create->getPosition().y + create->getGlobalBounds().height * 1.4f);
}
void SelectState::Init()
{
	InitBackground();
	InitUi();
}

void SelectState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void SelectState::HandleEvents(sf::Event& event)
{
	HandleUiEvents(event);
}

void SelectState::UpdateUi(const float& dt)
{
	m_UiManager.Update(dt);
}
void SelectState::Update(const float& dt)
{
	UpdateUi(dt);
}

void SelectState::RenderBackground(sf::RenderTarget* target)
{
	target->draw(m_Background);
}
void SelectState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void SelectState::Render(sf::RenderTarget* target)
{
	RenderUi(target);
}

void SelectState::End()
{
}