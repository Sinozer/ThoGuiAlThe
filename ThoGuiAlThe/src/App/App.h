#pragma once

class App
{
private:
	void m_InitWindow();
	void m_InitStates();

	void m_HandleEvents();
public:
	App();
	~App() = default;

	void Init();

	void Run();
	
	void End();
private:
	sf::RenderWindow m_Window;

	sf::Event m_Event;

	float m_DeltaTime;

private:
	StateManager* m_StateManager;

private:
	sf::Clock m_Clock;
	void m_UpdateDt();
};