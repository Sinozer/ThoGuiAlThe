#pragma once

class App
{
public:
	App();
	~App() = default;

	void Init();
	void Run();
	void End() const;
	
private:
	void InitWindow();
	void InitStates() const;
	void HandleEvents();
	void UpdateDt();
	
private:
	sf::RenderWindow m_Window;
	sf::Event m_Event;
	float m_DeltaTime;

	StateManager* m_StateManager;

	sf::Clock m_Clock;

};