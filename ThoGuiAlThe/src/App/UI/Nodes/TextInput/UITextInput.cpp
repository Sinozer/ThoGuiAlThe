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
		m_Focus = getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);

	if (!m_Focus)
		return;

	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode == 8 && getString().getSize() > 0)
			setString(getString().substring(0, getString().getSize() - 1));

		else if (event.text.unicode != 8)
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