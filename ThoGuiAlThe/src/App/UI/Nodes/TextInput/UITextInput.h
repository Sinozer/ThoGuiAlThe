#pragma once
#include "App/UI/Nodes/Text/UIText.h"

class UITextInput : public UIText
{
public:
	UITextInput();
	UITextInput(std::string text);
	~UITextInput() = default;

	void HandleEvents(sf::Event& event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

private:
	bool m_Focus = false;
	bool m_CursorVisible = false;
	sf::Clock m_CursorClock;
	const sf::Time m_BlinkInterval = sf::milliseconds(500);
};

