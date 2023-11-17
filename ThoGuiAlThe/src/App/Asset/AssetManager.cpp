#include "AssetManager.h"

#pragma region Singleton
AssetManager* AssetManager::m_sInstance = nullptr;

AssetManager* AssetManager::GetInstance()
{
	if (m_sInstance == nullptr)
		m_sInstance = new AssetManager();

	return m_sInstance;
}

void AssetManager::DestroyInstance()
{
	if (m_sInstance == nullptr)
		return;
	DELPTR(m_sInstance);
}
#pragma endregion

#pragma region Textures
sf::Texture& AssetManager::LoadTexture(const std::string& name, const std::string& fileName, const sf::IntRect area)
{
	if (!TextureExists(name))
	{
		sf::Texture texture;
		if (texture.loadFromFile(fileName, area))
			m_Textures[name] = texture;
	}
	return GetTexture(name);
}

sf::Texture& AssetManager::GetTexture(const std::string& name)
{
	if (!TextureExists(name))
		throw std::exception("Texture not found");

	return m_Textures.at(name);
}

bool AssetManager::TextureExists(const std::string& name) const
{
	return m_Textures.find(name) != m_Textures.end();
}
#pragma endregion

#pragma region Fonts
sf::Font& AssetManager::LoadFont(const std::string& name, const std::string& fileName)
{
	if (!FontExists(name))
	{
		sf::Font font;
		if (font.loadFromFile(fileName))
			m_Fonts[name] = font;
	}
	return GetFont(name);
}

sf::Font& AssetManager::GetFont(const std::string& name)
{
	if (!FontExists(name))
		throw std::exception("Font not found");

	return m_Fonts.at(name);
}

bool AssetManager::FontExists(const std::string& name) const
{
	return m_Fonts.find(name) != m_Fonts.end();
}
#pragma endregion