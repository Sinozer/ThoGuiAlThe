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
	auto* joinButton = m_UiManager.AddTextButton("JoinButton", "Join");

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
	idInput->SetOnValid([joinButton]() { joinButton->OnClick(); });

	joinButton->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - joinButton->getGlobalBounds().getSize().x / 2,
		WINDOW_SCREEN_HEIGHT / 1.5f + joinButton->getGlobalBounds().getSize().y / 2
	);
	joinButton->setCharacterSize(50);
	joinButton->setOutlineThickness(2.f);
	joinButton->setOutlineColor(sf::Color::Black);
	joinButton->SetCallback([idInput]()
		{
			int id = std::atoi(idInput->getString().toAnsiString().c_str());

			I(NetworkManager).SendData({
				{JSON_EVENT_TYPE, TgatClientMessage::JOIN_SESSION},
				{JSON_SESSION_ID, id}
				});
		});

	auto* randJoinButton = m_UiManager.AddTextButton("RandJoinButton", "Random");
	randJoinButton->setPosition(
		joinButton->getPosition().x - randJoinButton->getGlobalBounds().getSize().x / 2,
		joinButton->getPosition().y + joinButton->getGlobalBounds().getSize().y * 1.5f
	);
	randJoinButton->setCharacterSize(50);
	randJoinButton->setOutlineThickness(2.f);
	randJoinButton->setOutlineColor(sf::Color::Black);
	randJoinButton->SetCallback([]()
		{
			I(NetworkManager).SendData({
				{JSON_EVENT_TYPE, TgatClientMessage::JOIN_SESSION},
				{JSON_SESSION_ID, -1}
				});
		});

	auto* badId = m_UiManager.AddText("BadId", "Bad ID");
	badId->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - badId->getGlobalBounds().getSize().x / 2,
		WINDOW_SCREEN_HEIGHT / 1.5f + badId->getGlobalBounds().getSize().y / 2
	);
	badId->setFillColor(sf::Color::Red);
	badId->setOutlineColor(sf::Color::Black);
	badId->setOutlineThickness(2.f);
	badId->SetActive(false);
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

	NetworkManager& networkManager = I(NetworkManager);

	nlohmann::json data;
	if (networkManager.ReceiveData(TgatServerMessage::BAD_SESSION_ID, data) == false)
		return;

	m_UiManager.GetText("BadId")->SetActive(true);
}

void JoinState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void JoinState::Render(sf::RenderTarget* target)
{
	RenderUi(target);
}

void JoinState::Resume()
{
}

void JoinState::End()
{
}