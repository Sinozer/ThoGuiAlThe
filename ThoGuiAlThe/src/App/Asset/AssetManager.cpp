#include "AssetManager.h"

#pragma region Singleton
AssetManager* AssetManager::s_Instance = nullptr;

AssetManager* AssetManager::GetInstance()
{
	if (s_Instance == nullptr)
		s_Instance = new AssetManager();

	return s_Instance;
}

const void AssetManager::DestroyInstance()
{
	if (s_Instance == nullptr)
		return;
	delete s_Instance;
	s_Instance = nullptr;
}
#pragma endregion

#pragma region Textures
sf::Texture& AssetManager::LoadTexture(std::string name, std::string fileName, sf::IntRect area)
{
	if (!TextureExists(name))
	{
		sf::Texture texture;
		if (texture.loadFromFile(fileName, area))
			m_Textures[name] = texture;
	}
	return GetTexture(name);
}

sf::Texture& AssetManager::GetTexture(std::string name)
{
	if (!TextureExists(name))
		throw std::exception("Texture not found");

	return m_Textures.at(name);
}

const bool AssetManager::TextureExists(std::string name) const
{
	return m_Textures.find(name) != m_Textures.end();
}
#pragma endregion

#pragma region Fonts
sf::Font& AssetManager::LoadFont(std::string name, std::string fileName)
{
	if (!FontExists(name))
	{
		sf::Font font;
		if (font.loadFromFile(fileName))
			m_Fonts[name] = font;
	}
	return GetFont(name);
}

sf::Font& AssetManager::GetFont(std::string name)
{
	if (!FontExists(name))
		throw std::exception("Font not found");

	return m_Fonts.at(name);
}

const bool AssetManager::FontExists(std::string name) const
{
	return m_Fonts.find(name) != m_Fonts.end();
}
#pragma endregion