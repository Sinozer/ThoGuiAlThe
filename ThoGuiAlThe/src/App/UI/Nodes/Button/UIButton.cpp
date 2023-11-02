#include "stdafx.h"
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

void UIButton::Update(const float& dt)
{
}

void UIButton::Render(sf::RenderTarget* target)
{
	target->draw(*this);
}