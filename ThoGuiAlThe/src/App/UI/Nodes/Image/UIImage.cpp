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
}