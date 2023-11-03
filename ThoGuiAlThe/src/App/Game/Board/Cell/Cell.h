#pragma once

#include "App/Game/Player/Player.h"

class Cell : public sf::RectangleShape
{
public:
	Cell(sf::Vector2u position);
	~Cell();

	void Init();
	void HandleEvents(sf::Event* event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

	void SetPlayer(Player* player) { m_Player = player; };
	Player* GetPlayer() const { return m_Player; };

private:
	Player* m_Player;

private:
	sf::Vector2u m_Position;
};