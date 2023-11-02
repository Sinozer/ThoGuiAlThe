#pragma once

class HomeState : public State
{
private:
	void m_InitBackground();

	virtual void m_InitUI();
	virtual void m_UpdateUI(const float& dt);
	virtual void m_RenderUI(sf::RenderTarget* target);

public:
	HomeState();
	~HomeState() = default;

	void Init();
	void End();

	void HandleEvents(sf::Event& event);

	void Update(const float& dt);
	void Render(sf::RenderTarget* target = nullptr);
};