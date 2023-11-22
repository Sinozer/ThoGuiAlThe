#include "ConnectState.h"

#include "App/Network/NetworkManager.h"

#include "App/State/StateManager.h"
#include "App/State/List/Home/HomeState.h"

ConnectState::ConnectState() = default;


void ConnectState::InitBackground()
{
}
void ConnectState::InitUi()
{
	auto* connectText = m_UiManager.AddText("CONNECTING", "CONNECTING TO SERVER ...");
	connectText->setCharacterSize(15);
	connectText->setOutlineColor(sf::Color::Black);
	connectText->setOutlineThickness(1.f);
	connectText->setPosition(WINDOW_SCREEN_WIDTH / 2 - connectText->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - connectText->getGlobalBounds().height - 300.f);
}
void ConnectState::Init()
{
	InitBackground();
	InitUi();

	Sleep(1000);
}

void ConnectState::Resume()
{
}

void ConnectState::End()
{
}

void ConnectState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void ConnectState::HandleEvents(sf::Event& event)
{
	HandleUiEvents(event);
}

void ConnectState::UpdateUi(const float& dt)
{
	m_UiManager.Update(dt);
}
void ConnectState::Update(const float& dt)
{
	UpdateUi(dt);
	if (I(NetworkManager).IsInit())
	{
		I(StateManager)->AddState(new HomeState());
	}
}

void ConnectState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void ConnectState::Render(sf::RenderTarget* target)
{
	if (target == nullptr)
		return;

	target->draw(m_Background);

	RenderUi(target);
}