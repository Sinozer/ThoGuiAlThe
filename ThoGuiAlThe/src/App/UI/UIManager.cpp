#include "UIManager.h"

UIManager::~UIManager()
{
	for (auto& i : m_UiTexts)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_UiTexts.clear();

	for (auto& i : m_UiButtons)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_UiButtons.clear();
}

void UIManager::HandleEvents(sf::Event& event)
{
	for (auto& i : m_UiButtons)
	{
		i.second->HandleEvents(event);
	}
}

void UIManager::UpdateButtons(const float& dt)
{
	for (auto& i : m_UiButtons)
	{
		i.second->Update(dt);
	}
}
void UIManager::Update(const float& dt)
{
	UpdateButtons(dt);
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
	for (const auto& [_, second] : m_UiButtons)
	{
		second->Render(target);
	}
}
void UIManager::Render(sf::RenderTarget* target)
{
	RenderTexts(target);
	RenderButtons(target);
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

UIButton* UIManager::AddButton(const std::string& name)
{
	if (m_UiButtons.contains(name))
	{
		std::cout << "UIButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UIButton* newButton = new UIButton();
	m_UiButtons[name] = newButton;
	return newButton;
}
UIButton* UIManager::AddButton(const std::string& name, const std::string& text)
{
	if (m_UiButtons.contains(name))
	{
		std::cout << "UIButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UIButton* newButton = new UIButton(text);
	m_UiButtons[name] = newButton;
	return newButton;
}
UIButton* UIManager::AddButton(const std::string& name, const std::string& text, const std::function<void()>& callback)
{
	if (m_UiButtons.contains(name))
	{
		std::cout << "UIButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UIButton* newButton = new UIButton(text, callback);
	m_UiButtons[name] = newButton;
	return newButton;
}