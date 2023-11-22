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
#pragma region STATE_TITLE
	auto* title = m_UiManager.AddText("TITLE", "Profile");
	title->setCharacterSize(100);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2, 50.f);
#pragma endregion

	const PlayerDisplayData& playerDisplayData = I(NetworkManager).GetPlayerDisplayData();

#pragma region PROFILE_NAME
	// Name input
	auto* pName = m_UiManager.AddTextInput("PLAYER_NAME", playerDisplayData.name);
	pName->setCharacterSize(50);
	pName->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - pName->getGlobalBounds().getSize().x / 2,
		title->getPosition().y + title->getGlobalBounds().getSize().y + pName->getGlobalBounds().getSize().y * 1.5f
	);
	pName->setOutlineThickness(2.f);
	pName->setOutlineColor(sf::Color::Black);
#pragma endregion

#pragma region PROFILE_PICTURE
	auto* picture = m_UiManager.AddImage("PLAYER_PICTURE", playerDisplayData.profilePicturePath);
	picture->setPosition(
		WINDOW_SCREEN_WIDTH / 2.f - picture->getGlobalBounds().width / 2.f,
		pName->getPosition().y + pName->getGlobalBounds().getSize().y * 1.25f
	);

	auto* pictureLeftSelector = m_UiManager.AddTextButton("PICTURE_LEFT_SELECTOR", "<");
	pictureLeftSelector->setCharacterSize(50);
	pictureLeftSelector->setOutlineThickness(2.f);
	pictureLeftSelector->setOutlineColor(sf::Color::Black);
	pictureLeftSelector->SetCallback([this, picture]() {
		if (m_ActualIndex == 0)
			m_ActualIndex = PROFILE_PICTURES.size() - 1;
		else
			m_ActualIndex--;

		picture->setTexture(I(AssetManager)->GetTexture(PROFILE_PICTURES[m_ActualIndex]));
		});
	pictureLeftSelector->setPosition(
		picture->getPosition().x - pictureLeftSelector->getGlobalBounds().getSize().x * 1.5f,
		picture->getPosition().y + picture->getGlobalBounds().getSize().y / 2.f - pictureLeftSelector->getGlobalBounds().getSize().y / 2.f
	);

	auto* pictureRightSelector = m_UiManager.AddTextButton("PICTURE_RIGHT_SELECTOR", ">");
	pictureRightSelector->setCharacterSize(50);
	pictureRightSelector->setOutlineThickness(2.f);
	pictureRightSelector->setOutlineColor(sf::Color::Black);
	pictureRightSelector->SetCallback([this, picture]() {
		if (m_ActualIndex == PROFILE_PICTURES.size() - 1)
			m_ActualIndex = 0;
		else
			m_ActualIndex++;

		picture->setTexture(I(AssetManager)->GetTexture(PROFILE_PICTURES[m_ActualIndex]));
		});
	pictureRightSelector->setPosition(
		picture->getPosition().x + picture->getGlobalBounds().getSize().x + pictureRightSelector->getGlobalBounds().getSize().x / 2.f,
		picture->getPosition().y + picture->getGlobalBounds().getSize().y / 2.f - pictureRightSelector->getGlobalBounds().getSize().y / 2.f
	);
#pragma endregion

#pragma region SAVE_BUTTON
	auto* save = m_UiManager.AddTextButton("SaveBtn", "SAVE");
	save->setCharacterSize(50);
	save->setOutlineThickness(2.f);
	save->setOutlineColor(sf::Color::Black);
	save->SetCallback([this, pName]() {
		NetworkManager& networkManager = I(NetworkManager);
		User* user = networkManager.GetUser();

		user->SetName(pName->getString());

		nlohmann::json eventData =
		{
			{JSON_EVENT_TYPE, TgatClientMessage::PLAYER_CHANGE_INFO},
			{JSON_PLAYER_NAME, pName->getString().toAnsiString()},
			{JSON_PLAYER_PPP, PROFILE_PICTURES[m_ActualIndex]},
			{JSON_PLAYER_PPTP, PROFILE_PICTURES[m_ActualIndex] + "_THUMB"},
			//{JSON_PLAYER_COLOR, /*TODO*/}
		};
		TgatNetworkHelper::Message msg;
		std::string strData = eventData.dump();
		const int headerId = networkManager.HEADER_ID;
		const int playerId = networkManager.GetPlayerId();
		networkManager.CreateMessage(headerId, playerId, strData, msg);
		networkManager.Send(msg);
		});
	save->setPosition(
		WINDOW_SCREEN_WIDTH / 2 - save->getGlobalBounds().getSize().x / 2,
		WINDOW_SCREEN_HEIGHT - save->getGlobalBounds().getSize().y * 1.5f
	);
#pragma endregion
}
void ProfileState::Init()
{
	std::vector<std::string> names = { "DEFAULT", "HAZMAT", "HAZMAT2", "SHREK", "MARGE", "HOMER" };

	std::string actualName = I(NetworkManager).GetPlayerDisplayData().profilePicturePath;
	for (size_t i = 0; i < names.size(); i++)
	{
		if (names[i] == actualName)
		{
			m_ActualIndex = i;
			break;
		}
	}

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