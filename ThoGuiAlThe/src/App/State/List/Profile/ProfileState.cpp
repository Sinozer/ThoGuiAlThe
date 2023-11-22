#include "ProfileState.h"

#include "App/Network/NetworkManager.h"
#include "App/User/User.h"

ProfileState::ProfileState()
{
}

void ProfileState::InitBackground()
{
}

void ProfileState::InitUi()
{
#pragma region title
	auto* title = m_UiManager.AddText("TITLE", "Profile");
	title->setCharacterSize(100);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2, 100.f);
#pragma endregion

	const PlayerDisplayData& playerDisplayData = I(NetworkManager).GetPlayerDisplayData();

#pragma region input
	// Name input
	auto* pName = m_UiManager.AddTextInput("player name", playerDisplayData.name);
	pName->setPosition(
		WINDOW_SCREEN_WIDTH / 4 - pName->getGlobalBounds().getSize().x / 2,
		WINDOW_SCREEN_HEIGHT / 2 - pName->getGlobalBounds().getSize().y / 2
	);
	pName->setCharacterSize(50);
	pName->setOutlineThickness(2.f);
	pName->setOutlineColor(sf::Color::Black);

	// Color title
	auto* title = m_UiManager.AddText("Color", "Color");
	title->setCharacterSize(100);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2,
		pName->getPosition().y + pName->getGlobalBounds().getSize().y * 1.5f
	);


	auto* r = m_UiManager.AddTextInput("player name", playerDisplayData.name);
	r->setPosition(
		WINDOW_SCREEN_WIDTH / 4 - r->getGlobalBounds().getSize().x / 2,
		WINDOW_SCREEN_HEIGHT / 2 - r->getGlobalBounds().getSize().y / 2
	);
	r->setCharacterSize(50);
	r->setOutlineThickness(2.f);
	r->setOutlineColor(sf::Color::Black);
#pragma endregion

#pragma region profile picture
	auto* picture = m_UiManager.AddImage("PICTURE", playerDisplayData.profilePicturePath);
	picture->setPosition(
		WINDOW_SCREEN_WIDTH / 2.f - picture->getGlobalBounds().width / 2.f + pName->getGlobalBounds().getSize().x * 1.2f,
		WINDOW_SCREEN_HEIGHT / 2.f - picture->getGlobalBounds().height / 2.f
	);
#pragma endregion

#pragma region save
	auto* save = m_UiManager.AddTextButton("SaveBtn", "SAVE");
	save->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - save->getGlobalBounds().getSize().x / 2,
		WINDOW_SCREEN_HEIGHT / 1.5f + save->getGlobalBounds().getSize().y / 2
	);
	save->setCharacterSize(50);
	save->setOutlineThickness(2.f);
	save->setOutlineColor(sf::Color::Black);
	save->SetCallback([pName]() {
		NetworkManager& networkManager = I(NetworkManager);
		User* user = networkManager.GetUser();

		user->SetName(pName->getString());
		user->Set

			nlohmann::json eventData =
		{
			{JSON_EVENT_TYPE, TgatClientMessage::PLAYER_CHANGE_INFO},
			{JSON_PLAYER_NAME, idInput->getString().toAnsiString()},
		};
		TgatNetworkHelper::Message msg;
		std::string strData = eventData.dump();
		const int headerId = networkManager.HEADER_ID;
		const int playerId = networkManager.GetPlayerId();
		networkManager.CreateMessage(headerId, playerId, strData, msg);
		networkManager.Send(msg);
		});
#pragma endregion
}
void ProfileState::Init()
{
	InitBackground();
	InitUi();
}

void ProfileState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void ProfileState::HandleEvents(sf::Event& event)
{
	HandleUiEvents(event);

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			StateManager::GetInstance()->RemoveState();
		}
	}
}

void ProfileState::UpdateUi(const float& dt)
{
	m_UiManager.Update(dt);
}
void ProfileState::Update(const float& dt)
{
	UpdateUi(dt);
}

void ProfileState::RenderBackground(sf::RenderTarget* target)
{
}
void ProfileState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void ProfileState::Render(sf::RenderTarget* target)
{
	RenderBackground(target);
	RenderUi(target);
}

void ProfileState::End()
{
}