#pragma once
#include "App/UI/UIManager.h"

class State
{
protected:
	sf::Color m_ClearColor = sf::Color(38, 70, 83);
	sf::Sprite m_Background;

	virtual void m_InitBackground() = 0;

	UIManager m_UIManager;

	virtual void m_InitUI() = 0;
	virtual void m_UpdateUI(const float& dt) = 0;
	virtual void m_RenderUI(sf::RenderTarget* target) = 0;
public:
	const sf::Color GetClearColor() const { return m_ClearColor; }

public:
	State() = default;
	virtual ~State() = default;

	virtual void Init() = 0;
	virtual void End() = 0;

	virtual void HandleEvents(sf::Event& event) = 0;

	virtual void Update(const float& dt) = 0;
	virtual void Render(sf::RenderTarget* target = nullptr) = 0;
};;