#pragma once

#include "App/Game/Player/Player.h"

class Cell final : public sf::RectangleShape
{
public:
	explicit Cell(sf::Vector2u position);
	~Cell() override;

	void Init();
	void HandleEvents(sf::Event* event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

	void SetPlayer(Player* player) { m_Player = player; }
	Player* GetPlayer() const { return m_Player; }

private:
	Player* m_Player;

private:
	sf::Vector2u m_Position;
};