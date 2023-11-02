#pragma once

class GameState : public State
{
private:
	void m_InitBackground();

	virtual void m_InitUI();
	virtual void m_HandleUiEvents(sf::Event& event);
	virtual void m_UpdateUI(const float& dt);
	virtual void m_RenderUI(sf::RenderTarget* target);

public:
	GameState();
	~GameState() = default;

	void Init();
	void End();

	void HandleEvents(sf::Event& event);

	void Update(const float& dt);
	void Render(sf::RenderTarget* target = nullptr);
};