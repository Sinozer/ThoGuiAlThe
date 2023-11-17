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

	// Getters
	inline const bool& IsActive() const { return m_Active; }

	// Setters
	inline void SetActive(const bool& active) { m_Active = active; }
protected:
	bool m_Active = true;
};