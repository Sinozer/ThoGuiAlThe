#include "UIButton.h"

UIButton::UIButton()
{
	setString("DEFAULT STRING");
	m_Callback = []() {};
}

UIButton::UIButton(std::string text)
{
	setString(text);
	m_Callback = []() {};
}

UIButton::UIButton(std::string text, std::function<void()> callback)
{
	setString(text);
	m_Callback = callback;
}

void UIButton::HandleEvents(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
			{
				m_Callback();
			}
		}
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		/*******************************************************\
		|**	[TODO] - Make this more efficient and less costly **|
		\*******************************************************/
		if (getGlobalBounds().contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)))
		{
			setFillColor(sf::Color(255, 255, 255, 100));
		}
		else
		{
			setFillColor(sf::Color(255, 255, 255, 255));
		}
	}
}

void UIButton::Update(const float& dt)
{
}

void UIButton::Render(sf::RenderTarget* target)
{
	target->draw(*this);
}