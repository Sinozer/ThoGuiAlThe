#include "JoinState.h"
#include "App/State/List/Game/GameState.h"

#include "App/Network/NetworkManager.h"

JoinState::JoinState()
{
}

void JoinState::InitBackground()
{
}
void JoinState::InitUi()
{
	auto* idText = m_UiManager.AddText("ID", "Enter ID: ");
	idText->setPosition(
		WINDOW_SCREEN_WIDTH / 4 - idText->getGlobalBounds().getSize().x / 2,
		WINDOW_SCREEN_HEIGHT / 2 - idText->getGlobalBounds().getSize().y / 2
	);
	idText->setCharacterSize(50);
	idText->setOutlineThickness(2.f);
	idText->setOutlineColor(sf::Color::Black);

	auto* idInput = m_UiManager.AddTextInput("IDInput", "XXXXXXXX");
	idInput->setPosition(
		idText->getPosition().x + idText->getGlobalBounds().getSize().x * 1.1f,
		idText->getPosition().y
	);
	idInput->setCharacterSize(50);
	idInput->setOutlineThickness(2.f);
	idInput->setOutlineColor(sf::Color::Black);

	auto* joinButton = m_UiManager.AddTextButton("JoinButton", "Join");
	joinButton->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - joinButton->getGlobalBounds().getSize().x / 2,
		WINDOW_SCREEN_HEIGHT / 1.5f + joinButton->getGlobalBounds().getSize().y / 2
	);
	joinButton->setCharacterSize(50);
	joinButton->setOutlineThickness(2.f);
	joinButton->setOutlineColor(sf::Color::Black);
	joinButton->SetCallback([idInput]() {
		int id = std::atoi(idInput->getString().toAnsiString().c_str());

		NetworkManager& networkManager = I(NetworkManager);
		nlohmann::json eventData =
		{
			{JSON_EVENT_TYPE, TgatClientMessage::JOIN_SESSION},
			{JSON_SESSION_ID, id}
		};
		TgatNetworkHelper::Message msg;
		std::string strData = eventData.dump();
		const int headerId = networkManager.HEADER_ID;
		const int playerId = networkManager.GetPlayerId();
		networkManager.CreateMessage(headerId, playerId, strData, msg);
		networkManager.Send(msg);
	});
}
void JoinState::Init()
{
	InitBackground();
	InitUi();
}

void JoinState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void JoinState::HandleEvents(sf::Event& event)
{
	HandleUiEvents(event);

	if (event.type != sf::Event::KeyPressed)
		return;
	if (event.key.code != sf::Keyboard::Escape)
		return;

	StateManager::GetInstance()->RemoveState();
}

void JoinState::UpdateUi(const float& dt)
{
	m_UiManager.Update(dt);
}
void JoinState::Update(const float& dt)
{
	UpdateUi(dt);
}

void JoinState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void JoinState::Render(sf::RenderTarget* target)
{
	RenderUi(target);
}

void JoinState::End()
{
}