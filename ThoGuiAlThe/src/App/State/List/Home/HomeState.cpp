#include "HomeState.h"
#include "App/State/List/Profile/ProfileState.h"
#include "App/State/List/Select/SelectState.h"
#include "App/Network/NetworkManager.h"

#include "App/State/List/Game/GameState.h"

HomeState::HomeState()
= default;


void HomeState::InitBackground()
{
}
void HomeState::InitUi()
{
	auto* title = m_UiManager.AddText("TITLE", "THOGUIALTHE");
	title->setCharacterSize(100);
	title->setOutlineColor(sf::Color::Black);
	title->setOutlineThickness(4.f);
	title->setPosition(WINDOW_SCREEN_WIDTH / 2 - title->getGlobalBounds().width / 2, 100.f);

	auto* profile = m_UiManager.AddImageButton("PROFILE", "DEFAULT_THUMB", [this]() { StateManager::GetInstance()->AddState(new ProfileState()); });
	profile->setPosition(WINDOW_SCREEN_WIDTH * 0.99f - profile->getGlobalBounds().width, WINDOW_SCREEN_HEIGHT / 100.f);
	profile->SetOutlineThickness(3.f);
	profile->SetOutlineColor(sf::Color::White);

	auto* username = m_UiManager.AddTextInput("USERNAME", "{USERNAME}");
	username->setCharacterSize(15);
	username->setOutlineColor(sf::Color::Black);
	username->setOutlineThickness(1.f);
	username->setPosition(profile->getPosition().x - username->getGlobalBounds().width * 1.05f, profile->getPosition().y);

	auto* start = m_UiManager.AddTextButton("START", "PLAY", [this]() { StateManager::GetInstance()->AddState(new SelectState()); });
	start->setCharacterSize(50);
	start->setOutlineColor(sf::Color::Black);
	start->setOutlineThickness(2.f);
	start->setPosition(WINDOW_SCREEN_WIDTH / 2 - start->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - start->getGlobalBounds().height - 300.f);

	auto* exit = m_UiManager.AddTextButton("EXIT", "EXIT", [this]() { StateManager::GetInstance()->RemoveAllStates(); });
	exit->setOutlineColor(sf::Color::Black);
	exit->setOutlineThickness(2.f);
	exit->setPosition(WINDOW_SCREEN_WIDTH / 2 - exit->getGlobalBounds().width / 2, WINDOW_SCREEN_HEIGHT - exit->getGlobalBounds().height - 50.f);

	if (NetworkManager::GetInstance()->IsConnected())
		return;

	auto* networkConnectionFailedButton = m_UiManager.AddImageButton("NETWORK_CONNECTION_FAILED", "RETRY_THUMB");
	networkConnectionFailedButton->setPosition(WINDOW_SCREEN_WIDTH * 0.01f, WINDOW_SCREEN_HEIGHT / 100.f);

	auto* networkConnectionFailedText = m_UiManager.AddText("NETWORK_CONNECTION_FAILED", "NETWORK CONNECTION FAILED");
	networkConnectionFailedText->setCharacterSize(15);
	networkConnectionFailedText->setOutlineColor(sf::Color::Black);
	networkConnectionFailedText->setOutlineThickness(1.f);
	networkConnectionFailedText->setPosition(networkConnectionFailedButton->getPosition().x + networkConnectionFailedButton->getGlobalBounds().width * 1.05f, networkConnectionFailedButton->getPosition().y);

	networkConnectionFailedButton->SetCallback([this, networkConnectionFailedButton, networkConnectionFailedText]()
		{
			if (NetworkManager::GetInstance()->Connect())
			{
				networkConnectionFailedButton->SetActive(false);
				networkConnectionFailedText->SetActive(false);
			}
		}
	);
}
void HomeState::Init()
{
	InitBackground();
	InitUi();
}

void HomeState::End()
{
}

void HomeState::HandleUiEvents(sf::Event& event)
{
	m_UiManager.HandleEvents(event);
}
void HomeState::HandleEvents(sf::Event& event)
{
	HandleUiEvents(event);
}

void HomeState::UpdateUi(const float& dt)
{
	m_UiManager.Update(dt);
}
void HomeState::Update(const float& dt)
{
	UpdateUi(dt);
}

void HomeState::RenderUi(sf::RenderTarget* target)
{
	m_UiManager.Render(target);
}
void HomeState::Render(sf::RenderTarget* target)
{
	if (target == nullptr)
		return;

	target->draw(m_Background);

	RenderUi(target);
}