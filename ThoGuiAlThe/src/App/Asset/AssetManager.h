#pragma once

class AssetManager
{
#pragma region Singleton
public:
	static AssetManager* GetInstance();
	static const void DestroyInstance();
private:
	static AssetManager* s_Instance;
#pragma endregion

private:
	std::map<std::string, sf::Texture> m_Textures;
	std::map<std::string, sf::Font> m_Fonts;

public:
	AssetManager() = default;
	~AssetManager() = default;

	sf::Texture& LoadTexture(std::string name, std::string fileName, sf::IntRect area = sf::IntRect());
	sf::Texture& GetTexture(std::string name);
	const bool TextureExists(std::string name) const;

	sf::Font& LoadFont(std::string name, std::string fileName);
	sf::Font& GetFont(std::string name);
	const bool FontExists(std::string name) const;
};