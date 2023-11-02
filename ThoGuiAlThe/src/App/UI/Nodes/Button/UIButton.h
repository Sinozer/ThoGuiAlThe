#pragma once
#include "App/UI/Nodes/Text/UIText.h"

class UIButton : public UIText
{
public:
	UIButton();
	UIButton(std::string text);
	UIButton(std::string text, std::function<void()> callback);
	~UIButton() = default;

	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

private:
	std::function<void()> m_Callback;
};