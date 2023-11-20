#pragma once
#include "Nodes/Text/UIText.h"
#include "Nodes/Image/UIImage.h"
#include "Nodes/Button/TextButton/UITextButton.h"
#include "Nodes/Button/ImageButton/UIImageButton.h"
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

	UIImage* AddImage(const std::string& name);
	UIImage* AddImage(const std::string& name, const std::string& textureName);

	UITextButton* AddTextButton(const std::string& name);
	UITextButton* AddTextButton(const std::string& name, const std::string& text);
	UITextButton* AddTextButton(const std::string& name, const std::string& text, const std::function<void()>& callback);

	UIImageButton* AddImageButton(const std::string& name);
	UIImageButton* AddImageButton(const std::string& name, const std::string& textureName);
	UIImageButton* AddImageButton(const std::string& name, const std::string& textureName, const std::function<void()>& callback);

	UITextInput* AddTextInput(const std::string& name);
	UITextInput* AddTextInput(const std::string& name, const std::string& text);

	UIText* GetText(const std::string& name);
	UIImage* GetImage(const std::string& name);
	UITextButton* GetTextButton(const std::string& name);
	UIImageButton* GetImageButton(const std::string& name);
	UITextInput* GetTextInput(const std::string& name);

private:
	std::map<std::string, UIText*> m_UiTexts;
	std::map<std::string, UIImage*> m_UiImages;
	std::map<std::string, UITextButton*> m_UITextButtons;
	std::map<std::string, UIImageButton*> m_UIImageButtons;
	std::map<std::string, UITextInput*> m_UiTextInputs;

	void UpdateButtons(const float& dt);
	void UpdateTextInputs(const float& dt);

	void RenderTexts(sf::RenderTarget* target);
	void RenderImages(sf::RenderTarget* target);
	void RenderButtons(sf::RenderTarget* target);
	void RenderTextInputs(sf::RenderTarget* target);
};