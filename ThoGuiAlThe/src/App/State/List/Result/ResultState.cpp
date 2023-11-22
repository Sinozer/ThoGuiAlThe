#include "stdafx.h"
#include "ResultState.h"

#include "App/Network/NetworkManager.h"

ResultState::ResultState()
{
}

void ResultState::InitBackground()
{
}
void ResultState::InitUi()
{
	auto& q = I(NetworkManager).GetReceiveQueue(TgatServerMessage::GAME_END);
	if (q.empty()) return;

	nlohmann::json& data = q.front();

	int playerId = data[JSON_PLAYER_ID];

	std::string titleText;

	if (playerId == I(NetworkManager).GetPlayerId())
		titleText = "YOU WIN";
	else if (playerId != I(NetworkManager).GetPlayerId())
		titleText = "YOU LOSE";
	else
		titleText = "DRAW";

	q.pop();

	auto* title = m_UiManager.AddText("TITLE", titleText);
	title->setCharacterSize(100);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2, 100.f);

	auto* menu = m_UiManager.AddTextButton("MENU", "MENU", [this]() { StateManager::GetInstance()->GoToFirstState(); });
	menu->setOutlineColor(sf::Color::Black);
	menu->setOutlineThickness(2.f);
	menu->setPosition(WINDOW_SCREEN_WIDTH / 2 - menu->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - menu->getGlobalBounds().height - 50.f);

	auto* exit = m_UiManager.AddTextButton("EXIT", "EXIT", [this]() { StateManager::GetInstance()->RemoveAllStates(); });
	exit->setOutlineColor(sf::Color::Black);
	exit->setOutlineThickness(2.f);
	exit->setPosition(WINDOW_SCREEN_WIDTH / 2 - exit->getGlobalBounds().width / 2, menu->getPosition().y + menu->getGlobalBounds().height * 1.05f);
}
void ResultState::Init()
{
	InitBackground();
	InitUi();
}

void ResultState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void ResultState::HandleEvents(sf::Event& event)
{
	HandleUiEvents(event);
}

void ResultState::UpdateUi(const float& dt)
{
	m_UiManager.Update(dt);
}
void ResultState::Update(const float& dt)
{
	UpdateUi(dt);
}

void ResultState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void ResultState::Render(sf::RenderTarget* target)
{
	RenderUi(target);
}

void ResultState::End()
{
}