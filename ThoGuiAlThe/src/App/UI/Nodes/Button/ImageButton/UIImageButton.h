#pragma once
#include "App/UI/Nodes/Image/UIImage.h"

class UIImageButton : public UIImage
{
public:
	UIImageButton();
	UIImageButton(std::string textureName);
	UIImageButton(std::string textureName, std::function<void()> callback);
	~UIImageButton() = default;

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