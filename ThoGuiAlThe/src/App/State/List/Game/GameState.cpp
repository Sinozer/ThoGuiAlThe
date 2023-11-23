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

	const PlayerDisplayData& playerDisplayData = I(NetworkManager).GetPlayerDisplayData();

#pragma region P1
	auto* p1Profile = m_UiManager.AddImage("P1_PROFILE", playerDisplayData.profilePicturePath);
	p1Profile->SetOutlineThickness(4.f);
	p1Profile->SetOutlineColor(sf::Color(231, 111, 81));
	p1Profile->setPosition(
		WINDOW_SCREEN_WIDTH / 8 - p1Profile->getGlobalBounds().width / 2,
		WINDOW_SCREEN_HEIGHT / 2 - p1Profile->getGlobalBounds().height / 2
	);

	auto* p1Username = m_UiManager.AddText("P1_USERNAME", playerDisplayData.name);
	p1Username->setCharacterSize(25);
	p1Username->setOutlineColor(sf::Color::Black);
	p1Username->setOutlineThickness(2.f);
	p1Username->setPosition(
		p1Profile->getPosition().x + p1Profile->getGlobalBounds().width / 2 - p1Username->getGlobalBounds().width / 2,
		p1Profile->getPosition().y + p1Profile->getGlobalBounds().height * 1.1f
	);
#pragma endregion P1

#pragma region P2
	auto* p2Profile = m_UiManager.AddImage("P2_PROFILE", playerDisplayData.profilePicturePath);
	p2Profile->SetOutlineThickness(4.f);
	p2Profile->SetOutlineColor(sf::Color(42, 157, 143));
	p2Profile->setPosition(
		WINDOW_SCREEN_WIDTH / 8 *  - p2Profile->getGlobalBounds().width / 2,
		WINDOW_SCREEN_HEIGHT / 2 - p2Profile->getGlobalBounds().height / 2
	);

	auto* p2Username = m_UiManager.AddText("P2_USERNAME", playerDisplayData.name);
	p2Username->setCharacterSize(25);
	p2Username->setOutlineColor(sf::Color::Black);
	p2Username->setOutlineThickness(2.f);
	p2Username->setPosition(
		p2Profile->getPosition().x + p2Profile->getGlobalBounds().width / 2 - p2Username->getGlobalBounds().width / 2,
		p2Profile->getPosition().y + p2Profile->getGlobalBounds().height * 1.1f
	);
#pragma endregion P2
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
	if (networkManager.ReceiveData(TgatServerMessage::SESSION_LEFT, data) == true)
	{
		bool self = data[JSON_PLAYER_ID] == networkManager.GetPlayerId();

		if (self == true)
		{

		}
		else
		{

		}

		stateManager->GoToFirstState();
	}

		if (networkManager.ReceiveData(TgatServerMessage::PLAYER_INFO, data) == true)
	{
		auto* p2Profile = m_UiManager.GetImage("P2_PROFILE");
		auto* p2Username = m_UiManager.GetText("P2_USERNAME");

		p2Profile->setTexture(I(AssetManager)->GetTexture(data[JSON_PLAYER_PPP]));
		p2Username->setString((std::string)data[JSON_PLAYER_NAME]);

		p2Profile->setPosition(
			WINDOW_SCREEN_WIDTH / 8 * 7 - p2Profile->getGlobalBounds().width / 2,
			WINDOW_SCREEN_HEIGHT / 2 - p2Profile->getGlobalBounds().height / 2
		);

		p2Username->setPosition(
			p2Profile->getPosition().x + p2Profile->getGlobalBounds().width / 2 - p2Username->getGlobalBounds().width / 2,
			p2Profile->getPosition().y + p2Profile->getGlobalBounds().height * 1.1f
		);
	}
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