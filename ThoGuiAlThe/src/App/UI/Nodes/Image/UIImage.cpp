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
}

void UIImage::Render(sf::RenderTarget* target)
{
	target->draw(*this);
}