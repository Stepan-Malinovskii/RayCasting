#pragma once
#ifndef RESOURCES
#define RESOURCES

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio.hpp>
#include "array"

const float TEXTURE_SIZE = 64.0f, SPRITE_SIZE = 104.0f, GUN_SIZE = 1.0f;
constexpr float SCREEN_W = 1280.0f, SCREEN_H = 720.0f;

class Resources
{
public:
	static sf::Texture textures;
	static sf::Texture skyTextures;
	static sf::Image textureImage;
	static sf::Texture spritesTexture;

	static sf::SoundBuffer gun1ShutSound;
	static sf::SoundBuffer gun1CantShoutSound;
	static sf::SoundBuffer gun1ResetSound;
	static sf::Texture gun1BaseTexture;
	static std::array<sf::Texture, 4> gun1FireAnimationTexture;

	static sf::SoundBuffer gun2ShutSound;
	static sf::SoundBuffer gun2CantShoutSound;
	static sf::SoundBuffer gun2ResetSound;
	static sf::Texture gun2BaseTexture;
	static std::array<sf::Texture, 4> gun2FireAnimationTexture;

	static sf::Font UIFont;
};

#endif // !RESOURCES
