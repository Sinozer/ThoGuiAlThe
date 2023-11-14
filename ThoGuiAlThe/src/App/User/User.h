#pragma once
class User
{
public:
	User() = default;
	~User() = default;

private:
	std::string m_Name = "USER";
	
	std::string m_ProfilePictureName = "THUMB_DEFAULT";
	sf::Color m_ProfilePictureOutlineColor = sf::Color::Transparent;
};

