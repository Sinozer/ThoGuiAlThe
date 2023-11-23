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
	nlohmann::json data;

	if (I(NetworkManager).ReceiveData(TgatServerMessage::GAME_END, data) == true)
	{
		int playerId = data[JSON_PLAYER_ID];

		std::string titleText;

		if (playerId == I(NetworkManager).GetPlayerId())
			titleText = "YOU WIN";
		else if (playerId != I(NetworkManager).GetPlayerId())
			titleText = "YOU LOSE";
		else
			titleText = "DRAW";

		auto* title = m_UiManager.AddText("TITLE", titleText);
		title->setCharacterSize(100);
		title->setOutlineColor(sf::Color::Black);
		title->setOutlineThickness(4.f);
		title->setPosition(WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2, 100.f);

		auto* replayButton = m_UiManager.AddTextButton("REPLAY_BUTTON", "REPLAY");
		replayButton->setOutlineColor(sf::Color::Black);
		replayButton->setOutlineThickness(2.f);
		replayButton->setCharacterSize(50);
		replayButton->setPosition(
			WINDOW_SCREEN_WIDTH / 2 - replayButton->getGlobalBounds().width / 2,
			WINDOW_SCREEN_HEIGHT / 2 - replayButton->getGlobalBounds().height / 2
		);
		replayButton->SetCallback([this]()
			{
				I(NetworkManager).SendData(
					{
						{JSON_EVENT_TYPE, TgatClientMessage::REPLAY},
						{JSON_PLAYER_ID, I(NetworkManager).GetPlayerId()},
						{JSON_SESSION_ID, I(NetworkManager).GetSessionId()}
					}
				);
			});

		auto* replayText = m_UiManager.AddText("REPLAY_TEXT", "0 / 2");
		replayText->setCharacterSize(35);
		replayText->setOutlineColor(sf::Color::Black);
		replayText->setOutlineThickness(2.f);
		replayText->setPosition(
			replayButton->getPosition().x + replayButton->getGlobalBounds().width / 2 - replayText->getGlobalBounds().width / 2,
			replayButton->getPosition().y + replayButton->getGlobalBounds().height * 1.05f
		);

		auto* menu = m_UiManager.AddTextButton("MENU", "MENU", [this]() { StateManager::GetInstance()->GoToFirstState(); });
		menu->setOutlineColor(sf::Color::Black);
		menu->setOutlineThickness(2.f);
		menu->setPosition(WINDOW_SCREEN_WIDTH / 2 - menu->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - menu->getGlobalBounds().height - 50.f);

		auto* exit = m_UiManager.AddTextButton("EXIT", "EXIT", [this]() { StateManager::GetInstance()->RemoveAllStates(); });
		exit->setOutlineColor(sf::Color::Black);
		exit->setOutlineThickness(2.f);
		exit->setPosition(WINDOW_SCREEN_WIDTH / 2 - exit->getGlobalBounds().width / 2, menu->getPosition().y + menu->getGlobalBounds().height * 1.05f);
	}
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

	nlohmann::json data;

	if (I(NetworkManager).ReceiveData(TgatServerMessage::GAME_REPLAY, data) == false)
		return;

	TGATPLAYERID playerId = data[JSON_PLAYER_ID];

	auto* replayText = m_UiManager.GetText("REPLAY_TEXT");
	replayText->setString("1 / 2");

	if (playerId == I(NetworkManager).GetPlayerId())
	{
		auto* replayButton = m_UiManager.GetTextButton("REPLAY_BUTTON");
		replayButton->SetActive(false);
	}
}

void ResultState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void ResultState::Render(sf::RenderTarget* target)
{
	RenderUi(target);
}

void ResultState::Resume()
{
}

void ResultState::End()
{
}