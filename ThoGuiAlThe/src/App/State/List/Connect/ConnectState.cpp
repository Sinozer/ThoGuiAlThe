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
	auto* ipText = m_UiManager.AddText("IP", "IP : ");
	ipText->setPosition(
		WINDOW_SCREEN_WIDTH / 4 - ipText->getGlobalBounds().width / 2,
		WINDOW_SCREEN_HEIGHT / 2 - ipText->getGlobalBounds().height / 2 - 50.f
	);

	auto* connectButton = m_UiManager.AddTextButton("CONNECT", "CONNECT");

	auto* ipInput = m_UiManager.AddTextInput("IP_INPUT", "Try something like '192.168.0.1'");
	ipInput->setPosition(
		ipText->getPosition().x + ipText->getGlobalBounds().width + 10.f,
		ipText->getPosition().y
	);
	ipInput->SetOnValid([connectButton]() {connectButton->OnClick(); });

	connectButton->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - connectButton->getGlobalBounds().width / 2,
		WINDOW_SCREEN_HEIGHT / 2 - connectButton->getGlobalBounds().height / 2 + 50.f
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