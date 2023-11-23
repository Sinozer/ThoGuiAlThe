#include "UITextInput.h"

UITextInput::UITextInput()
{
	setString("DEFAULT STRING");
	setFont(AssetManager::GetInstance()->GetFont("DEFAULT"));
}

UITextInput::UITextInput(std::string text)
{
	setString(text);
	setFont(AssetManager::GetInstance()->GetFont("DEFAULT"));
}

void UITextInput::HandleEvents(sf::Event& event)
{
	if (m_Active == false)
		return;

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		m_Focus = getGlobalBounds().contains((float)event.mouseButton.x, (float)event.mouseButton.y);

	if (!m_Focus)
		return;

	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode == 8)				// Backspace
		{
			if (getString().getSize() > 0)
				setString(getString().substring(0, getString().getSize() - 1));
			return;
		}

		else if (event.text.unicode == 13)			// Enter
		{
			Validate();
			return;
		}

		else if (event.text.unicode == 127)			// CTRL + Backspace
		{
			std::string text = getString();

			size_t endPos = text.find_last_not_of(" ");
			if (endPos == std::string::npos) {
				text.clear();
				setString(text);
				return;
			}

			// Find the position of the space before the word to delete
			size_t startPos = text.find_last_of(" ", endPos);
			if (startPos == std::string::npos) {
				text.clear();
			}
			else {
				text.erase(startPos);
			}
			setString(text);
			return;
		}

		else										// Any other character
			if (event.text.unicode < 32 || event.text.unicode > 127)	// Ignore non-printable characters
				return;

		setString(getString() + event.text.unicode);
	}
}

void UITextInput::Update(const float& dt)
{
	if (m_Active == false)
		return;

	if (m_CursorClock.getElapsedTime() >= m_BlinkInterval)
	{
		m_CursorVisible = !m_CursorVisible;
		m_CursorClock.restart();
	}
}

void UITextInput::Render(sf::RenderTarget* target)
{
	if (m_Active == false)
		return;

	target->draw(*this);

	if (m_Focus && m_CursorVisible)
	{
		sf::RectangleShape cursor;
		cursor.setSize(sf::Vector2f(1.5f, getGlobalBounds().height * 1.2f));
		cursor.setFillColor(sf::Color::Black);
		cursor.setPosition(sf::Vector2f(getGlobalBounds().left + getGlobalBounds().width, getGlobalBounds().top));
		target->draw(cursor);
	}
}