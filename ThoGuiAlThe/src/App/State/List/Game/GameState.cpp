#include "GameState.h"
#include "App/Network/NetworkManager.h"

GameState::GameState()
{
}
GameState::~GameState()
{
	DELPTR(m_Game);
}

void GameState::InitBackground()
{
}
void GameState::InitUi()
{
	auto* title = m_UiManager.AddText("TITLE", "TICTACTOE");
	title->setCharacterSize(100);
	title->setFillColor(sf::Color(38, 70, 83));
	title->setOutlineColor(sf::Color::White);
	title->setOutlineThickness(4.f);
	title->setPosition(
		(WINDOW_SCREEN_WIDTH - title->getGlobalBounds().width) / 2,
		100.f
	);

}
void GameState::InitGame()
{
	m_Game = new Game();
	m_Game->Init();
}
void GameState::Init()
{
	m_ClearColor = sf::Color(233, 196, 106);
	InitBackground();
	InitUi();
	InitGame();
}

void GameState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void GameState::HandleGameEvents(sf::Event& event)
{
	m_Game->HandleEvents(&event);
}
void GameState::HandleEvents(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		if (event.key.code == sf::Keyboard::Escape)
		{
			NetworkManager& networkManager = I(NetworkManager);
			networkManager.SendData(
				{
					{JSON_EVENT_TYPE, TgatClientMessage::LEAVE_SESSION},
					{JSON_SESSION_ID, networkManager.GetSessionId()},
					{JSON_PLAYER_ID, networkManager.GetPlayerId()}
				});
		}
	HandleUiEvents(event);
	HandleGameEvents(event);
}

void GameState::UpdateUi(const float& dt)
{
	m_UiManager.Update(dt);
}
void GameState::UpdateGame(const float& dt)
{
	m_Game->Update(dt);
}
void GameState::Update(const float& dt)
{
	UpdateUi(dt);
	UpdateGame(dt);

	StateManager* stateManager = I(StateManager);
	NetworkManager& networkManager = I(NetworkManager);

	nlohmann::json data;
	if (networkManager.ReceiveData(TgatServerMessage::SESSION_LEFT, data) == false)
		return;

	bool self = data[JSON_PLAYER_ID] == networkManager.GetPlayerId();

	if (self == true)
	{

	}
	else
	{

	}

	stateManager->GoToFirstState();
}

void GameState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void GameState::RenderGame(sf::RenderTarget* target)
{
	m_Game->Render(target);
}
void GameState::Render(sf::RenderTarget* target)
{
	if (target == nullptr)
		return;

	target->draw(m_Background);

	RenderUi(target);
	RenderGame(target);
}

void GameState::Resume()
{
}

void GameState::End()
{
}