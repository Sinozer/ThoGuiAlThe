#include "UIImageButton.h"

UIImageButton::UIImageButton()
{
	setTexture(AssetManager::GetInstance()->GetTexture("DEFAULT"), true);
	m_Callback = []() {};
}

UIImageButton::UIImageButton(std::string textureName)
{
	try
	{
		setTexture(AssetManager::GetInstance()->GetTexture(textureName), true);
	}
	catch (const std::exception&)
	{
		setTexture(AssetManager::GetInstance()->GetTexture("DEFAULT"), true);
	}
	m_Callback = []() {};
}

UIImageButton::UIImageButton(std::string textureName, std::function<void()> callback)
{
	try
	{
		setTexture(AssetManager::GetInstance()->GetTexture(textureName), true);
	}
	catch (const std::exception&)
	{
		setTexture(AssetManager::GetInstance()->GetTexture("DEFAULT"), true);
	}
	m_Callback = callback;
}

void UIImageButton::HandleEvents(sf::Event& event)
{
	if (m_Active == false)
		return;

	auto bounds = getGlobalBounds();
	sf::Vector2f mouseMovePos = sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y);
	sf::Vector2f mouseClickPos = sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
	bool mouseInMoveBounds = bounds.contains(mouseMovePos);
	bool mouseInClickBounds = bounds.contains(mouseClickPos);

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (mouseInClickBounds)
		{
			m_Callback();
		}
	}
	//else if (event.type == sf::Event::MouseMoved)
	//{
	//	/****************************************************\
	//	|**	[TODO] - Make this more efficient and costless **|
	//	\****************************************************/
	//	if (mouseInMoveBounds)
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
	if (m_Active == false)
		return;
}

void UIImageButton::Render(sf::RenderTarget* target)
{
	if (m_Active == false)
		return;

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