#pragma once

#include "App/Game/Game.h"

class GameState final : public State
{
public:
	GameState();
	~GameState() override;

	void Init() override;
	void HandleEvents(sf::Event& event) override;
	void Update(const float& dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;

	void End() override;
	
private:
	void InitBackground() override;

	void InitUi() override;
	void HandleUiEvents(sf::Event& event) override;
	void UpdateUi(const float& dt) override;
	void RenderUi(sf::RenderTarget* target) override;

	void InitGame();
	void HandleGameEvents(sf::Event& event);
	void UpdateGame(const float& dt);
	void RenderGame(sf::RenderTarget* target);

private:
	Game* m_Game;
};