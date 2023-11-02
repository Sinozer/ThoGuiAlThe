#pragma once
#include "Nodes/Text/UIText.h"
#include "Nodes/Button/UIButton.h"

class UIManager
{
public:
	UIManager() = default;
	~UIManager();

	void HandleEvents(sf::Event& event);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);

private:
	std::map<std::string, UIText*> m_UITexts;
	std::map<std::string, UIButton*> m_UIButtons;

	void m_UpdateButtons(const float& dt);

	void m_RenderTexts(sf::RenderTarget* target);
	void m_RenderButtons(sf::RenderTarget* target);

public:
	UIText* AddText(std::string name);
	UIText* AddText(std::string name, std::string text);

	UIButton* AddButton(std::string name);
	UIButton* AddButton(std::string name, std::string text);
	UIButton* AddButton(std::string name, std::string text, std::function<void()> callback);

};