#include "UIImageButton.h"

UIImageButton::UIImageButton()
{
	setTexture(AssetManager::GetInstance()->GetTexture("DEFAULT"), true);
	m_Callback = []() {};
}

UIImageButton::UIImageButton(std::string textureName)
{
	setTexture(AssetManager::GetInstance()->GetTexture(textureName), true);
	m_Callback = []() {};
}

UIImageButton::UIImageButton(std::string textureName, std::function<void()> callback)
{
	setTexture(AssetManager::GetInstance()->GetTexture(textureName), true);
	m_Callback = callback;
}

void UIImageButton::HandleEvents(sf::Event& event)
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
	//else if (event.type == sf::Event::MouseMoved)
	//{
	//	/****************************************************\
	//	|**	[TODO] - Make this more efficient and costless **|
	//	\****************************************************/
	//	if (mouseInBounds)
	//	{
	//		setColor(sf::Color(255, 255, 255, 100));
	//	}
	//	else
	//	{
	//		setColor(sf::Color(255, 255, 255, 255));
	//	}
	//}
}

void UIImageButton::Update(const float& dt)
{
}

void UIImageButton::Render(sf::RenderTarget* target)
{
	target->draw(*this);

	if (m_OutlineThickness > 0.f)
	{
		sf::RectangleShape outline;
		outline.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
		outline.setFillColor(sf::Color::Transparent);
		outline.setOutlineColor(m_OutlineColor);
		outline.setOutlineThickness(m_OutlineThickness);
		outline.setPosition(getPosition());

		target->draw(outline);
	}
}

void UIImageButton::SetCallback(std::function<void()> callback)
{
	m_Callback = callback;
}

void UIImageButton::SetOutlineThickness(float thickness)
{
	m_OutlineThickness = thickness;
}

void UIImageButton::SetOutlineColor(sf::Color color)
{
	m_OutlineColor = color;
}