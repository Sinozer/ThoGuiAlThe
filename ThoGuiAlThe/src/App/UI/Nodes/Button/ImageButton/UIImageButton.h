#pragma once
#include "App/UI/Nodes/Button/UIButton.h"
#include "App/UI/Nodes/Image/UIImage.h"

class UIImageButton : public UIButton, public UIImage
{
public:
	UIImageButton();
	UIImageButton(std::string textureName);
	UIImageButton(std::string textureName, std::function<void()> callback);
	~UIImageButton() = default;

	void HandleEvents(sf::Event& event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);
};