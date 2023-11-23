#include "UIImage.h"

UIImage::UIImage()
{
	setTexture(AssetManager::GetInstance()->GetTexture("DEFAULT"));
}

UIImage::UIImage(std::string texturePath)
{
	setTexture(AssetManager::GetInstance()->GetTexture(texturePath));
}

void UIImage::Update(const float& dt)
{
	if (m_Active == false)
		return;
}

void UIImage::Render(sf::RenderTarget* target)
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

void UIImage::SetOutlineThickness(float thickness)
{
	m_OutlineThickness = thickness;
}

void UIImage::SetOutlineColor(sf::Color color)
{
	m_OutlineColor = color;
}