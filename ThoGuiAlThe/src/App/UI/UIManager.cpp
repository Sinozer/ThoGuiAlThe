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

	for (auto& i : m_UIImageButtons)
	{
		DELPTR(i.second);
	}
	m_UIImageButtons.clear();
}

void UIManager::HandleEvents(sf::Event& event)
{
	for (auto& i : m_UITextButtons)
	{
		i.second->HandleEvents(event);
	}

	for (auto& i : m_UIImageButtons)
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

	for (auto& i : m_UIImageButtons)
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

	for (const auto& [_, second] : m_UIImageButtons)
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

	m_UiTexts[name] = new UIText();
	return m_UiTexts[name];
}
UIText* UIManager::AddText(const std::string& name, const std::string& text)
{
	if (m_UiTexts.contains(name))
	{
		std::cout << "UIText with name: " << name << " already exists!" << std::endl;
		return m_UiTexts[name];
	}

	m_UiTexts[name] = new UIText(text);
	return m_UiTexts[name];
}

UIImage* UIManager::AddImage(const std::string& name)
{
	if (m_UiImages.contains(name))
	{
		std::cout << "UIImage with name: " << name << " already exists!" << std::endl;
		return m_UiImages[name];
	}

	m_UiImages[name] = new UIImage();
	return m_UiImages[name];
}
UIImage* UIManager::AddImage(const std::string& name, const std::string& textureName)
{
	if (m_UiImages.contains(name))
	{
		std::cout << "UIImage with name: " << name << " already exists!" << std::endl;
		return m_UiImages[name];
	}
	
	m_UiImages[name] = new UIImage(textureName);
	return m_UiImages[name];
}

UITextButton* UIManager::AddTextButton(const std::string& name)
{
	if (m_UITextButtons.contains(name))
	{
		std::cout << "UITextButton with name: " << name << " already exists!" << std::endl;
		return m_UITextButtons[name];
	}

	m_UITextButtons[name] = new UITextButton();
	return m_UITextButtons[name];
}
UITextButton* UIManager::AddTextButton(const std::string& name, const std::string& text)
{
	if (m_UITextButtons.contains(name))
	{
		std::cout << "UITextButton with name: " << name << " already exists!" << std::endl;
		return m_UITextButtons[name];
	}

	m_UITextButtons[name] = new UITextButton(text);
	return m_UITextButtons[name];
}
UITextButton* UIManager::AddTextButton(const std::string& name, const std::string& text, const std::function<void()>& callback)
{
	if (m_UITextButtons.contains(name))
	{
		std::cout << "UIButton with name: " << name << " already exists!" << std::endl;
		return m_UITextButtons[name];
	}

	m_UITextButtons[name] = new UITextButton(text, callback);
	return m_UITextButtons[name];
}

UIImageButton* UIManager::AddImageButton(const std::string& name)
{
	if (m_UIImageButtons.contains(name))
	{
		std::cout << "UITextureButton with name: " << name << " already exists!" << std::endl;
		return m_UIImageButtons[name];
	}

	m_UIImageButtons[name] = new UIImageButton();
	return m_UIImageButtons[name];
}
UIImageButton* UIManager::AddImageButton(const std::string& name, const std::string& textureName)
{
	if (m_UIImageButtons.contains(name))
	{
		std::cout << "UITextureButton with name: " << name << " already exists!" << std::endl;
		return m_UIImageButtons[name];
	}

	m_UIImageButtons[name] = new UIImageButton(textureName);
	return m_UIImageButtons[name];
}
UIImageButton* UIManager::AddImageButton(const std::string& name, const std::string& textureName, const std::function<void()>& callback)
{
	if (m_UIImageButtons.contains(name))
	{
		std::cout << "UITextureButton with name: " << name << " already exists!" << std::endl;
		return m_UIImageButtons[name];
	}

	m_UIImageButtons[name] = new UIImageButton(textureName, callback);
	return m_UIImageButtons[name];
}

UITextInput* UIManager::AddTextInput(const std::string& name)
{
	if (m_UiTextInputs.contains(name))
	{
		std::cout << "UITextInput with name: " << name << " already exists!" << std::endl;
		return m_UiTextInputs[name];
	}

	m_UiTextInputs[name] = new UITextInput();
	return m_UiTextInputs[name];
}
UITextInput* UIManager::AddTextInput(const std::string& name, const std::string& text)
{
	if (m_UiTextInputs.contains(name))
	{
		std::cout << "UITextInput with name: " << name << " already exists!" << std::endl;
		return m_UiTextInputs[name];
	}

	m_UiTextInputs[name] = new UITextInput(text);
	return m_UiTextInputs[name];
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
	if (m_UIImageButtons.contains(name))
	{
		return m_UIImageButtons[name];
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