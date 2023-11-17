#pragma once
#include "App/UI/Nodes/UIElements.h"

class UIImage : public UIElement, public sf::Sprite
{
public:
	UIImage();
	UIImage(std::string texturePath);
	~UIImage() = default;

	void Update(const float& dt);
	void Render(sf::RenderTarget* target);
};