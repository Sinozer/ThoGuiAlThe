#include "UITextButton.h"

UITextButton::UITextButton()
{
	setString("DEFAULT STRING");
	m_Callback = []() {};
}

UITextButton::UITextButton(std::string text)
{
	setString(text);
	m_Callback = []() {};
}

UITextButton::UITextButton(std::string text, std::function<void()> callback)
{
	setString(text);
	m_Callback = callback;
}

void UITextButton::HandleEvents(sf::Event& event)
{
	auto bounds = getGlobalBounds();
	sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
	bool mouseInBounds = bounds.contains(mousePos);

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (mouseInBounds)
		{
			m_Callback();
		}
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		/****************************************************\
		|**	[TODO] - Make this more efficient and costless **|
		\****************************************************/
		if (mouseInBounds)
		{
			setFillColor(sf::Color(255, 255, 255, 100));
		}
		else
		{
			setFillColor(sf::Color(255, 255, 255, 255));
		}
	}
}

void UITextButton::Update(const float& dt)
{
}

void UITextButton::Render(sf::RenderTarget* target)
{
	target->draw(*this);
}