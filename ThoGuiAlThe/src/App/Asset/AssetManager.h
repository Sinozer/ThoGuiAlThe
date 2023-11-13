#pragma once

class AssetManager
{
#pragma region Singleton
public:
	static AssetManager* GetInstance();
	static void DestroyInstance();
private:
	static AssetManager* m_sInstance;
#pragma endregion

public:
	AssetManager() = default;
	~AssetManager() = default;

	sf::Texture& LoadTexture(const std::string& name, const std::string& fileName, sf::IntRect area = sf::IntRect());
	sf::Texture& GetTexture(const std::string& name);
	bool TextureExists(const std::string& name) const;

	sf::Font& LoadFont(const std::string& name, const std::string& fileName);
	sf::Font& GetFont(const std::string& name);
	bool FontExists(const std::string& name) const;
	
private:
	std::map<std::string, sf::Texture> m_Textures;
	std::map<std::string, sf::Font> m_Fonts;
};