#pragma once
#ifndef RESOURCES
#define RESOURCES

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
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
	static sf::Texture playerGunTexture;
	static sf::Texture gun1BaseTexture;
	static std::array<sf::Texture, 4> gun1FireAnimationTexture;
};

#endif // !RESOURCES
