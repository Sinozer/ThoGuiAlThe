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
	/*auto* connectText = m_UiManager.AddText("CONNECTING", "CONNECTING TO SERVER ...");
	connectText->setCharacterSize(15);
	connectText->setOutlineColor(sf::Color::Black);
	connectText->setOutlineThickness(1.f);
	connectText->setPosition(WINDOW_SCREEN_WIDTH / 2 - connectText->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - connectText->getGlobalBounds().height - 300.f);*/

	auto* ipText = m_UiManager.AddText("IP", "IP : ");
	ipText->setPosition(
		WINDOW_SCREEN_WIDTH / 4 - ipText->getGlobalBounds().width / 2,
		WINDOW_SCREEN_HEIGHT / 2 - ipText->getGlobalBounds().height / 2 - 50.f
	);

	auto* ipInput = m_UiManager.AddTextInput("IP_INPUT");
	ipInput->setPosition(
		ipText->getPosition().x + ipText->getGlobalBounds().width + 10.f,
		ipText->getPosition().y
	);

	auto* connectButton = m_UiManager.AddTextButton("CONNECT", "CONNECT");
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

	NetworkManager& networkManager = I(NetworkManager);

	auto& q = networkManager.GetReceiveQueue(TgatServerMessage::PLAYER_INIT);

	if (q.empty() == true)
		return;

	nlohmann::json& data = q.front();

	networkManager.InitPlayerWithData(data);

	q.pop();

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