#pragma once

/*
*	Abstract class for all UI elements
*	
*	Defaults:
*		- Position: 0, 0
*		- Size: 0, 0
*/

class UIElement
{
public:
	UIElement() = default;
	virtual ~UIElement() = default;

	virtual void Update(const float& dt) = 0;
	virtual void Render(sf::RenderTarget* target) = 0;
};