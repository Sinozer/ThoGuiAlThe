#include "ProfileState.h"

ProfileState::ProfileState()
{
}

void ProfileState::InitBackground()
{
}
void ProfileState::InitUi()
{
	auto* title = m_UiManager.AddText("TITLE", "Profile");
	title->setCharacterSize(100);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2, 100.f);
}
void ProfileState::Init()
{
	InitBackground();
	InitUi();
}

void ProfileState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void ProfileState::HandleEvents(sf::Event& event)
{
	HandleUiEvents(event);
}

void ProfileState::UpdateUi(const float& dt)
{
	m_UiManager.Update(dt);
}
void ProfileState::Update(const float& dt)
{
	UpdateUi(dt);
}

void ProfileState::RenderBackground(sf::RenderTarget* target)
{
}
void ProfileState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void ProfileState::Render(sf::RenderTarget* target)
{
	RenderBackground(target);
	RenderUi(target);
}

void ProfileState::End()
{
}