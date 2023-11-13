#include "stdafx.h"
#include "App.h"

#include "App/State/List/Home/HomeState.h"

App::App() : m_Event(), m_DeltaTime(0.f), m_StateManager(StateManager::GetInstance())
{
}

void App::InitWindow()
{
	m_Window.create(
		sf::VideoMode(WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT),
		"ThoGuiAlThe", sf::Style::Close | sf::Style::Titlebar
	);
	m_Window.setFramerateLimit(WINDOW_SCREEN_FRAMERATE_MED);
}
void App::InitStates() const
{
	m_StateManager->AddState(new HomeState());
}
void App::Init()
{
	InitWindow();
	InitStates();

	AssetManager::GetInstance()->LoadFont("DEFAULT", "assets/fonts/super_squad/super_squad.ttf");
}

void App::HandleEvents()
{
	while (m_Window.pollEvent(m_Event))
	{
		if (m_Event.type == sf::Event::Closed)
			m_Window.close();

		m_StateManager->GetActiveState()->HandleEvents(m_Event);
	}
}
void App::Run()
{
	while (m_Window.isOpen())
	{
		UpdateDt();

		m_StateManager->ProcessStateChanges();

		if (m_StateManager->IsEmpty()) break;

		HandleEvents();
		m_StateManager->GetActiveState()->Update(m_DeltaTime);

		m_Window.clear(m_StateManager->GetActiveState()->GetClearColor());

		m_StateManager->GetActiveState()->Render(&m_Window);

		m_Window.display();
	}
}

void App::End() const
{
	m_StateManager->RemoveAllStates();
	
	SingletonManager::DestroyAllInstances();
}

void App::UpdateDt()
{
	m_DeltaTime = m_Clock.restart().asSeconds();
}