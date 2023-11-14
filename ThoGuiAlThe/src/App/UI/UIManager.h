#pragma once
#include "Nodes/Text/UIText.h"
#include "Nodes/Button/TextButton/UITextButton.h"
#include "Nodes/Button/TextureButton/UITextureButton.h"
#include "Nodes/TextInput/UITextInput.h"

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

	UITextButton* AddTextButton(const std::string& name);
	UITextButton* AddTextButton(const std::string& name, const std::string& text);
	UITextButton* AddTextButton(const std::string& name, const std::string& text, const std::function<void()>& callback);

	UITextureButton* AddTextureButton(const std::string& name);
	UITextureButton* AddTextureButton(const std::string& name, const std::string& textureName);
	UITextureButton* AddTextureButton(const std::string& name, const std::string& textureName, const std::function<void()>& callback);

	UITextInput* AddTextInput(const std::string& name);
	UITextInput* AddTextInput(const std::string& name, const std::string& text);

private:
	std::map<std::string, UIText*> m_UiTexts;
	std::map<std::string, UITextButton*> m_UITextButtons;
	std::map<std::string, UITextureButton*> m_UITextureButtons;
	std::map<std::string, UITextInput*> m_UiTextInputs;

	void UpdateButtons(const float& dt);
	void UpdateTextInputs(const float& dt);

	void RenderTexts(sf::RenderTarget* target);
	void RenderButtons(sf::RenderTarget* target);
	void RenderTextInputs(sf::RenderTarget* target);
};