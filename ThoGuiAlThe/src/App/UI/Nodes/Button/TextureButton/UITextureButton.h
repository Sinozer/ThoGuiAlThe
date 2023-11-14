#pragma once
#include "App/UI/Nodes/Text/UIText.h"

class UITextureButton : public sf::Sprite
{
public:
	UITextureButton();
	UITextureButton(std::string textureName);
	UITextureButton(std::string textureName, std::function<void()> callback);
	~UITextureButton() = default;

	void HandleEvents(sf::Event& event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

public:
	void SetCallback(std::function<void()> callback);
	void SetOutlineThickness(float thickness);
	void SetOutlineColor(sf::Color color);

private:
	std::function<void()> m_Callback;

	float m_OutlineThickness = 0.f;
	sf::Color m_OutlineColor = sf::Color::Black;
};