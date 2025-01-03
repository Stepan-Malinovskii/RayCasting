#pragma once
#ifndef RESOURCES
#define RESOURCES

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>

const float TEXTURE_SIZE = 64.0f;

class Resources
{
public:
	static sf::Texture textures;
	static sf::Texture skyTextures;
	static sf::Image textureImage;
	static sf::Texture spritesTexture;
	static sf::Texture playerGunTexture;
};

#endif // !RESOURCES
