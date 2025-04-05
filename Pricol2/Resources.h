#pragma once
#ifndef RESOURCES
#define RESOURCES

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio.hpp>
#include "array"

const int TEXTURE_SIZE = 128, GUN_SIZE = 1, ICON_SIZE = 64, TEXTURE_COUNT = 15;
constexpr float SCREEN_W = 1280.0f, SCREEN_H = 720.0f;
constexpr int SPACE_SIZE_W = 40, SPACE_SIZE_H = 40;

class Resources
{
private:
	static void loadGun(std::string baseName, int index);
public:
	static sf::Texture dialogBackround;
	static sf::Texture textures;
	static sf::Texture skyTextures;
	static sf::Image textureImage;
	static sf::Texture spriteIcon;
	static std::vector<sf::Texture> spritesTextures;
	static sf::SoundBuffer takeDamage;
	static sf::SoundBuffer buttonClick;

	static sf::Texture itembleIcon;
	static sf::SoundBuffer gunCantShoutSound;

	static std::vector<std::vector<sf::Texture>> gunsResetAnim;
	static std::vector<std::vector<sf::Texture>> gunsFireAnim;
	static std::vector<sf::Texture> gunsBaseText;
	static std::vector<sf::SoundBuffer> gunsShutSound;
	static std::vector<sf::SoundBuffer> gunsResetSound;

	static sf::Font UIFont;

	static void initResources();
};

#endif // !RESOURCES
