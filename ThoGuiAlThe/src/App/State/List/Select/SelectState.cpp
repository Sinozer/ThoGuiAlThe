#include "SelectState.h"
#include "App/State/List/Game/GameState.h"
#include "App/State/List/Create/CreateState.h"
#include "App/State/List/Join/JoinState.h"
#include "App/Network/NetworkManager.h"

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

	auto* create = m_UiManager.AddTextButton("CREATE", "CREATE", [this]() { StateManager::GetInstance()->AddState(new CreateState()); });
	create->setCharacterSize(50);
	create->setOutlineColor(sf::Color::Black);
	create->setOutlineThickness(2.f);
	create->setPosition(WINDOW_SCREEN_WIDTH / 2 - create->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - create->getGlobalBounds().height - 350.f);

	auto* join = m_UiManager.AddTextButton("JOIN", "JOIN", [this]() { StateManager::GetInstance()->AddState(new JoinState()); });
	join->setCharacterSize(50);
	join->setOutlineColor(sf::Color::Black);
	join->setOutlineThickness(2.f);
	join->setPosition(WINDOW_SCREEN_WIDTH / 2 - join->getGlobalBounds().width / 2, create->getPosition().y + create->getGlobalBounds().height * 1.4f);


	auto* bypass = m_UiManager.AddTextButton("BYPASS", "BYPASS", [this]() 
		{ 
			StateManager::GetInstance()->AddState(new GameState());
			NetworkManager& networkManager = NetworkManager::GetInstance();
			nlohmann::json eventData = 
			{
				{JSON_EVENT_TYPE, TgatClientMessage::CREATE_SESSION}
			};
			TgatNetworkHelper::Message msg;
			std::string strData = eventData.dump();
			const int headerId = networkManager.HEADER_ID;
			const int playerId = networkManager.GetPlayerId();
			networkManager.CreateMessage(headerId, playerId, strData, msg);
			networkManager.Send(msg);
		});
	bypass->setCharacterSize(50);
	bypass->setOutlineColor(sf::Color::Black);
	bypass->setOutlineThickness(2.f);
	bypass->setPosition(WINDOW_SCREEN_WIDTH / 2 - bypass->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - bypass->getGlobalBounds().height - 100.f);
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