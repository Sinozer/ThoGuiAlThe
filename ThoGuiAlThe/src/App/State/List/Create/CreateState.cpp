#include "CreateState.h"
#include "App/Network/NetworkManager.h"

CreateState::CreateState()
{
}

void CreateState::InitBackground()
{
}
void CreateState::InitUi()
{
	auto* id = m_UiManager.AddText("ID", "Session id: " + std::to_string((int)I(NetworkManager).GetSessionId()));
	id->setPosition(
		WINDOW_SCREEN_WIDTH / 4 - id->getGlobalBounds().width / 2,
		WINDOW_SCREEN_HEIGHT / 4 - id->getGlobalBounds().height / 2
	);
	id->setCharacterSize(50);
	id->setOutlineThickness(2.f);
	id->setOutlineColor(sf::Color::Black);

	auto* waiting = m_UiManager.AddText("Waiting", "Waiting for other player...");
	waiting->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - waiting->getGlobalBounds().width / 2,
		WINDOW_SCREEN_HEIGHT / 2 - waiting->getGlobalBounds().height / 2
	);
	waiting->setCharacterSize(30);
	waiting->setOutlineThickness(2.f);
	waiting->setOutlineColor(sf::Color::Black);
}
void CreateState::Init()
{
	InitBackground();
	InitUi();
}

void CreateState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void CreateState::HandleEvents(sf::Event& event)
{
	HandleUiEvents(event);

	if (event.type != sf::Event::KeyPressed)
		return;
	if (event.key.code != sf::Keyboard::Escape)
		return;

	StateManager::GetInstance()->RemoveState(); // Delete session on server
}

void CreateState::UpdateUi(const float& dt)
{
}
void CreateState::Update(const float& dt)
{
	UpdateUi(dt);
}

void CreateState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void CreateState::Render(sf::RenderTarget* target)
{
	RenderUi(target);
}

void CreateState::Resume()
{
}

void CreateState::End()
{
}