#pragma once
#include "App/UI/UIManager.h"

class State
{
protected:
	sf::Color m_ClearColor = sf::Color(38, 70, 83);
	sf::Sprite m_Background;

	virtual void InitBackground() = 0;

	UIManager m_UiManager;

	virtual void InitUi() = 0;
	virtual void HandleUiEvents(sf::Event& event) = 0;
	virtual void UpdateUi(const float& dt) = 0;
	virtual void RenderUi(sf::RenderTarget* target) = 0;
public:
	sf::Color GetClearColor() const { return m_ClearColor; }

public:
	State() = default;
	virtual ~State() = default;

	virtual void Init() = 0;
	virtual void End() = 0;

	virtual void HandleEvents(sf::Event& event) = 0;

	virtual void Update(const float& dt) = 0;
	virtual void Render(sf::RenderTarget* target = nullptr) = 0;
};;