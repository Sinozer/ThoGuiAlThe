#pragma once
#include "App/UI/Nodes/UIElements.h"

class UIText : public UIElement, public sf::Text
{
public:
	UIText();
	UIText(std::string text);
	~UIText() = default;

	void Update(const float& dt);
	void Render(sf::RenderTarget* target);
};