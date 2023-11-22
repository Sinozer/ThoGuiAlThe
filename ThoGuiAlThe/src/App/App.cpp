#include "App.h"

#include "Network/NetworkManager.h"
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
	NetworkManager::GetInstance().Connect();
	InitWindow();
	InitStates();

	AssetManager* assetManager = AssetManager::GetInstance();

	assetManager->LoadFont("DEFAULT", "assets/fonts/super_squad/super_squad.ttf");

	assetManager->LoadTexture("DEFAULT", "assets/textures/angry_winnie.png");
	assetManager->LoadTexture("DEFAULT_THUMB", "assets/textures/angry_winnie_thumb.png");

	assetManager->LoadTexture("HAZMAT", "assets/textures/hazmat.png");
	assetManager->LoadTexture("HAZMAT_THUMB", "assets/textures/hazmat_thumb.png");

	assetManager->LoadTexture("HAZMAT2", "assets/textures/hazmat2.png");
	assetManager->LoadTexture("HAZMAT2_THUMB", "assets/textures/hazmat2_thumb.png");

	assetManager->LoadTexture("SHREK", "assets/textures/shrek.png");
	assetManager->LoadTexture("SHREK_THUMB", "assets/textures/shrek_thumb.png");

	assetManager->LoadTexture("MARGE", "assets/textures/marge.png");
	assetManager->LoadTexture("MARGE_THUMB", "assets/textures/marge_thumb.png");

	assetManager->LoadTexture("HOMER", "assets/textures/homer.png");
	assetManager->LoadTexture("HOMER_THUMB", "assets/textures/homer_thumb.png");

	assetManager->LoadTexture("RETRY_THUMB", "assets/textures/retry_thumb.png");
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
	I(NetworkManager).Close();
}

void App::End() const
{
	m_StateManager->RemoveAllStates();
	NetworkManager::GetInstance().Disconnect();
	SingletonManager::DestroyAllInstances();
}

void App::UpdateDt()
{
	m_DeltaTime = m_Clock.restart().asSeconds();
}