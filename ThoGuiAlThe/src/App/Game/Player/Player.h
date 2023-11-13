#pragma once

class Player
{
private:
	sf::Color m_Color;

public:
	explicit Player(sf::Color color);
	~Player() = default;

	void Init();
	void HandleEvents(sf::Event* event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

	sf::Color GetColor() const { return m_Color; }
};