#include "UIManager.h"

UIManager::~UIManager()
{
	for (auto& i : m_UiTexts)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_UiTexts.clear();

	for (auto& i : m_UITextButtons)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_UITextButtons.clear();

	for (auto& i : m_UiTextInputs)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_UiTextInputs.clear();

	for (auto& i : m_UITextureButtons)
	{
		delete i.second;
		i.second = nullptr;
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
	RenderButtons(target);
	RenderTextInputs(target);
}


UIText* UIManager::AddText(const std::string& name)
{
	if (m_UiTexts.contains(name))
	{
		std::cout << "UIText with name: " << name << " already exists!" << std::endl;
		return nullptr;
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
		return nullptr;
	}

	UIText* newText = new UIText(text);
	m_UiTexts[name] = newText;
	return newText;
}

UITextButton* UIManager::AddTextButton(const std::string& name)
{
	if (m_UITextButtons.contains(name))
	{
		std::cout << "UITextButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
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
		return nullptr;
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
		return nullptr;
	}

	UITextButton* newTextButton = new UITextButton(text, callback);
	m_UITextButtons[name] = newTextButton;
	return newTextButton;
}

UITextureButton* UIManager::AddTextureButton(const std::string& name)
{
	if (m_UITextureButtons.contains(name))
	{
		std::cout << "UITextureButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UITextureButton* newTextureButton = new UITextureButton();
	m_UITextureButtons[name] = newTextureButton;
	return newTextureButton;
}
UITextureButton* UIManager::AddTextureButton(const std::string& name, const std::string& textureName)
{
	if (m_UITextureButtons.contains(name))
	{
		std::cout << "UITextureButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UITextureButton* newTextureButton = new UITextureButton(textureName);
	m_UITextureButtons[name] = newTextureButton;
	return newTextureButton;
}
UITextureButton* UIManager::AddTextureButton(const std::string& name, const std::string& textureName, const std::function<void()>& callback)
{
	if (m_UITextureButtons.contains(name))
	{
		std::cout << "UITextureButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UITextureButton* newTextureButton = new UITextureButton(textureName, callback);
	m_UITextureButtons[name] = newTextureButton;
	return newTextureButton;
}

UITextInput* UIManager::AddTextInput(const std::string& name)
{
	if (m_UiTextInputs.contains(name))
	{
		std::cout << "UITextInput with name: " << name << " already exists!" << std::endl;
		return nullptr;
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
		return nullptr;
	}

	UITextInput* newTextInput = new UITextInput(text);
	m_UiTextInputs[name] = newTextInput;
	return newTextInput;
}