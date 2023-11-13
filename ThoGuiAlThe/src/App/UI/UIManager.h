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

	UIText* AddText(const std::string& name);
	UIText* AddText(const std::string& name, const std::string& text);

	UIButton* AddButton(const std::string& name);
	UIButton* AddButton(const std::string& name, const std::string& text);
	UIButton* AddButton(const std::string& name, const std::string& text, const std::function<void()>& callback);

private:
	std::map<std::string, UIText*> m_UiTexts;
	std::map<std::string, UIButton*> m_UiButtons;

	void UpdateButtons(const float& dt);

	void RenderTexts(sf::RenderTarget* target);
	void RenderButtons(sf::RenderTarget* target);
};