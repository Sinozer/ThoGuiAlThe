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

public:
	void SetDefaultText(std::string text) { m_DefaultText = text; }
	std::string GetDefaultText() { return m_DefaultText; }
private:
	std::string m_DefaultText;

public:
	void SetOnValid(std::function<void()> onValid) { m_OnValid = onValid; }
	void Validate() { if (m_OnValid) m_OnValid(); }
private:
	std::function<void()> m_OnValid;
};

