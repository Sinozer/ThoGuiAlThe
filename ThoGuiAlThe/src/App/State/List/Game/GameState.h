#pragma once

#include "App/Game/Game.h"

class GameState : public State
{
private:
	void m_InitBackground();

	virtual void m_InitUI();
	virtual void m_HandleUiEvents(sf::Event& event);
	virtual void m_UpdateUI(const float& dt);
	virtual void m_RenderUI(sf::RenderTarget* target);

private:
	Game* m_Game;

	void m_InitGame();
	void m_HandleGameEvents(sf::Event& event);
	void m_UpdateGame(const float& dt);
	void m_RenderGame(sf::RenderTarget* target);

public:
	GameState();
	~GameState();

	void Init();
	void HandleEvents(sf::Event& event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target = nullptr);

	void End();
};