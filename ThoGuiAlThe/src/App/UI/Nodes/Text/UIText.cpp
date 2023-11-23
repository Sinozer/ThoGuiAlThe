#include "UIText.h"

UIText::UIText()
{
	setString("DEFAULT STRING");
	setFont(AssetManager::GetInstance()->GetFont("DEFAULT"));
}

UIText::UIText(std::string text)
{
	setString(text);
	setFont(AssetManager::GetInstance()->GetFont("DEFAULT"));
}

void UIText::Update(const float& dt)
{
	if (m_Active == false)
		return;
}

void UIText::Render(sf::RenderTarget* target)
{
	if (m_Active == false)
		return;

	target->draw(*this);
}