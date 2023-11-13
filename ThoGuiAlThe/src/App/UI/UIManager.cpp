#include "UIManager.h"

UIManager::~UIManager()
{
	for (auto& i : m_UITexts)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_UITexts.clear();

	for (auto& i : m_UIButtons)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_UIButtons.clear();
}

void UIManager::HandleEvents(sf::Event& event)
{
	for (auto& i : m_UIButtons)
	{
		i.second->HandleEvents(event);
	}
}

void UIManager::m_UpdateButtons(const float& dt)
{
	for (auto& i : m_UIButtons)
	{
		i.second->Update(dt);
	}
}
void UIManager::Update(const float& dt)
{
	m_UpdateButtons(dt);
}

void UIManager::m_RenderTexts(sf::RenderTarget* target)
{
	for (auto& i : m_UITexts)
	{
		i.second->Render(target);
	}
}
void UIManager::m_RenderButtons(sf::RenderTarget* target)
{
	for (auto& i : m_UIButtons)
	{
		i.second->Render(target);
	}
}
void UIManager::Render(sf::RenderTarget* target)
{
	m_RenderTexts(target);
	m_RenderButtons(target);
}


UIText* UIManager::AddText(std::string name)
{
	if (m_UITexts.find(name) != m_UITexts.end())
	{
		std::cout << "UIText with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UIText* newText = new UIText();
	m_UITexts[name] = newText;
	return newText;
}
UIText* UIManager::AddText(std::string name, std::string text)
{
	if (m_UITexts.find(name) != m_UITexts.end())
	{
		std::cout << "UIText with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UIText* newText = new UIText(text);
	m_UITexts[name] = newText;
	return newText;
}

UIButton* UIManager::AddButton(std::string name)
{
	if (m_UIButtons.find(name) != m_UIButtons.end())
	{
		std::cout << "UIButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UIButton* newButton = new UIButton();
	m_UIButtons[name] = newButton;
	return newButton;
}
UIButton* UIManager::AddButton(std::string name, std::string text)
{
	if (m_UIButtons.find(name) != m_UIButtons.end())
	{
		std::cout << "UIButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UIButton* newButton = new UIButton(text);
	m_UIButtons[name] = newButton;
	return newButton;
}
UIButton* UIManager::AddButton(std::string name, std::string text, std::function<void()> callback)
{
	if (m_UIButtons.find(name) != m_UIButtons.end())
	{
		std::cout << "UIButton with name: " << name << " already exists!" << std::endl;
		return nullptr;
	}

	UIButton* newButton = new UIButton(text, callback);
	m_UIButtons[name] = newButton;
	return newButton;
}