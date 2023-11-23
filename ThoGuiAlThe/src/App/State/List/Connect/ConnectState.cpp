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
	auto* title = m_UiManager.AddText("TITLE", "Connect");
	title->setCharacterSize(100);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2, 100.f);

	auto* ipText = m_UiManager.AddText("IP", "IP : ");
	ipText->setCharacterSize(50);
	ipText->setOutlineColor(sf::Color::Black);
	ipText->setOutlineThickness(2.f);
	ipText->setPosition(
		WINDOW_SCREEN_WIDTH / 3 - ipText->getGlobalBounds().width / 2,
		WINDOW_SCREEN_HEIGHT / 2
	);

	auto* connectButton = m_UiManager.AddTextButton("CONNECT", "CONNECT");

	auto* ipInput = m_UiManager.AddTextInput("IP_INPUT");
	ipInput->setString("localhost");
	ipInput->setCharacterSize(50);
	ipInput->setOutlineColor(sf::Color::Black);
	ipInput->setOutlineThickness(2.f);
	ipInput->setPosition(
		ipText->getPosition().x + ipText->getGlobalBounds().width + 10.f,
		ipText->getPosition().y + ipText->getGlobalBounds().height - ipInput->getGlobalBounds().height
	);
	ipInput->SetOnValid([connectButton]() {connectButton->OnClick(); });

	connectButton->setCharacterSize(50);
	connectButton->setOutlineColor(sf::Color::Black);
	connectButton->setOutlineThickness(2.f);
	connectButton->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - connectButton->getGlobalBounds().width / 2,
		WINDOW_SCREEN_HEIGHT - connectButton->getGlobalBounds().height - 50.f
	);
	connectButton->SetCallback([ipInput]()
		{
			I(NetworkManager).Connect(ipInput->getString());
		});
}
void ConnectState::Init()
{
	InitBackground();
	InitUi();
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

	NetworkManager& networkManager = I(NetworkManager);

	nlohmann::json data;
	if (networkManager.ReceiveData(TgatServerMessage::PLAYER_INIT, data) == false)
		return;

	networkManager.InitPlayerWithData(data);

	StateManager* stateManager = I(StateManager);

	stateManager->RemoveAllStates();
	stateManager->AddState(new HomeState());
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