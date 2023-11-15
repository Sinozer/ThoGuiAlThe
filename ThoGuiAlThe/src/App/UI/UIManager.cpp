#include "UIManager.h"

UIManager::~UIManager()
{
	for (auto& i : m_UiTexts)
	{
		DELPTR(i.second);
	}
	m_UiTexts.clear();

	for (auto& i : m_UiImages)
	{
		DELPTR(i.second);
	}

	for (auto& i : m_UITextButtons)
	{
		DELPTR(i.second);
	}
	m_UITextButtons.clear();

	for (auto& i : m_UiTextInputs)
	{
		DELPTR(i.second);
	}
	m_UiTextInputs.clear();

	for (auto& i : m_UITextureButtons)
	{
		DELPTR(i.second);
	}
	m_UITextureButtons.clear();
}

void UIManager::HandleEvents(sf::Event& event)
{
	for (auto& i : m_UITextButtons)
	{
		i.second->HandleEvents(event);
	}

	for (auto& i : m_UITextureButtons)
	{
		i.second->HandleEvents(event);
	}

	for (auto& i : m_UiTextInputs)
	{
		i.second->HandleEvents(event);
	}
}

void UIManager::UpdateButtons(const float& dt)
{
	for (auto& i : m_UITextButtons)
	{
		i.second->Update(dt);
	}

	for (auto& i : m_UITextureButtons)
	{
		i.second->Update(dt);
	}
}
void UIManager::UpdateTextInputs(const float& dt)
{
	for (auto& i : m_UiTextInputs)
	{
		i.second->Update(dt);
	}
}
void UIManager::Update(const float& dt)
{
	UpdateButtons(dt);
	UpdateTextInputs(dt);
}

void UIManager::RenderTexts(sf::RenderTarget* target)
{
	for (auto& i : m_UiTexts)
	{
		i.second->Render(target);
	}
}
void UIManager::RenderImages(sf::RenderTarget* target)
{
	for (auto& i : m_UiImages)
	{
		i.second->Render(target);
	}
}
void UIManager::RenderButtons(sf::RenderTarget* target)
{
	for (const auto& [_, second] : m_UITextButtons)
	{
		second->Render(target);
	}

	for (const auto& [_, second] : m_UITextureButtons)
	{
		second->Render(target);
	}
}
void UIManager::RenderTextInputs(sf::RenderTarget* target)
{
	for (auto& i : m_UiTextInputs)
	{
		i.second->Render(target);
	}
}
void UIManager::Render(sf::RenderTarget* target)
{
	RenderTexts(target);
	RenderImages(target);
	RenderButtons(target);
	RenderTextInputs(target);
}


UIText* UIManager::AddText(const std::string& name)
{
	if (m_UiTexts.contains(name))
	{
		std::cout << "UIText with name: " << name << " already exists!" << std::endl;
		return m_UiTexts[name];
	}

	UIText* newText = new UIText();
	m_UiTexts[name] = newText;
	return newText;
}
UIText* UIManager::AddText(const std::string& name, const std::string& text)
{
	if (m_UiTexts.contains(name))
	{
		std::cout << "UIText with name: " << name << " already exists!" << std::endl;
		return m_UiTexts[name];
	}

	UIText* newText = new UIText(text);
	m_UiTexts[name] = newText;
	return newText;
}

UIImage* UIManager::AddImage(const std::string& name)
{
	if (m_UiImages.contains(name))
	{
		std::cout << "UIImage with name: " << name << " already exists!" << std::endl;
		return m_UiImages[name];
	}

	UIImage* newImage = new UIImage();
	m_UiImages[name] = newImage;
	return newImage;
}
UIImage* UIManager::AddImage(const std::string& name, const std::string& textureName)
{
	if (m_UiImages.contains(name))
	{
		std::cout << "UIImage with name: " << name << " already exists!" << std::endl;
		return m_UiImages[name];
	}

	UIImage* newImage = new UIImage(textureName);
	m_UiImages[name] = newImage;
	return newImage;
}

UITextButton* UIManager::AddTextButton(const std::string& name)
{
	if (m_UITextButtons.contains(name))
	{
		std::cout << "UITextButton with name: " << name << " already exists!" << std::endl;
		return m_UITextButtons[name];
	}

	UITextButton* newTextButton = new UITextButton();
	m_UITextButtons[name] = newTextButton;
	return newTextButton;
}
UITextButton* UIManager::AddTextButton(const std::string& name, const std::string& text)
{
	if (m_UITextButtons.contains(name))
	{
		std::cout << "UITextButton with name: " << name << " already exists!" << std::endl;
		return m_UITextButtons[name];
	}

	UITextButton* newTextButton = new UITextButton(text);
	m_UITextButtons[name] = newTextButton;
	return newTextButton;
}
UITextButton* UIManager::AddTextButton(const std::string& name, const std::string& text, const std::function<void()>& callback)
{
	if (m_UITextButtons.contains(name))
	{
		std::cout << "UIButton with name: " << name << " already exists!" << std::endl;
		return m_UITextButtons[name];
	}

	UITextButton* newTextButton = new UITextButton(text, callback);
	m_UITextButtons[name] = newTextButton;
	return newTextButton;
}

UIImageButton* UIManager::AddImageButton(const std::string& name)
{
	if (m_UITextureButtons.contains(name))
	{
		std::cout << "UITextureButton with name: " << name << " already exists!" << std::endl;
		return m_UITextureButtons[name];
	}

	UIImageButton* newTextureButton = new UIImageButton();
	m_UITextureButtons[name] = newTextureButton;
	return newTextureButton;
}
UIImageButton* UIManager::AddImageButton(const std::string& name, const std::string& textureName)
{
	if (m_UITextureButtons.contains(name))
	{
		std::cout << "UITextureButton with name: " << name << " already exists!" << std::endl;
		return m_UITextureButtons[name];
	}

	UIImageButton* newTextureButton = new UIImageButton(textureName);
	m_UITextureButtons[name] = newTextureButton;
	return newTextureButton;
}
UIImageButton* UIManager::AddImageButton(const std::string& name, const std::string& textureName, const std::function<void()>& callback)
{
	if (m_UITextureButtons.contains(name))
	{
		std::cout << "UITextureButton with name: " << name << " already exists!" << std::endl;
		return m_UITextureButtons[name];
	}

	UIImageButton* newTextureButton = new UIImageButton(textureName, callback);
	m_UITextureButtons[name] = newTextureButton;
	return newTextureButton;
}

UITextInput* UIManager::AddTextInput(const std::string& name)
{
	if (m_UiTextInputs.contains(name))
	{
		std::cout << "UITextInput with name: " << name << " already exists!" << std::endl;
		return m_UiTextInputs[name];
	}

	UITextInput* newTextInput = new UITextInput();
	m_UiTextInputs[name] = newTextInput;
	return newTextInput;
}
UITextInput* UIManager::AddTextInput(const std::string& name, const std::string& text)
{
	if (m_UiTextInputs.contains(name))
	{
		std::cout << "UITextInput with name: " << name << " already exists!" << std::endl;
		return m_UiTextInputs[name];
	}

	UITextInput* newTextInput = new UITextInput(text);
	m_UiTextInputs[name] = newTextInput;
	return newTextInput;
}

UIText* UIManager::GetText(const std::string& name)
{
	if (m_UiTexts.contains(name))
	{
		return m_UiTexts[name];
	}

	std::cout << "UIText with name: " << name << " does not exist!" << std::endl;
	return nullptr;

}

UITextButton* UIManager::GetTextButton(const std::string& name)
{
	if (m_UITextButtons.contains(name))
	{
		return m_UITextButtons[name];
	}

	std::cout << "UITextButton with name: " << name << " does not exist!" << std::endl;
	return nullptr;
}

UIImageButton* UIManager::GetImageButton(const std::string& name)
{
	if (m_UITextureButtons.contains(name))
	{
		return m_UITextureButtons[name];
	}

	std::cout << "UITextureButton with name: " << name << " does not exist!" << std::endl;
	return nullptr;
}

UITextInput* UIManager::GetTextInput(const std::string& name)
{
	if (m_UiTextInputs.contains(name))
	{
		return m_UiTextInputs[name];
	}

	std::cout << "UITextInput with name: " << name << " does not exist!" << std::endl;
	return nullptr;
}