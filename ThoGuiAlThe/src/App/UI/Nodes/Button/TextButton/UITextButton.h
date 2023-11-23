#pragma once
#include "App/UI/Nodes/Button/UIButton.h"
#include "App/UI/Nodes/Text/UIText.h"

class UITextButton : public UIButton, public UIText
{
public:
	UITextButton();
	UITextButton(std::string text);
	UITextButton(std::string text, std::function<void()> callback);
	~UITextButton() = default;

	void HandleEvents(sf::Event& event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);
};