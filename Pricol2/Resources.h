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
	static sf::Texture gun1IconTexture;
	static std::vector<sf::Texture> gun1FireAnimationTexture;
	static std::vector<sf::Texture> gun1ResetAnimationTexture;

	static sf::SoundBuffer gun2ShutSound;
	static sf::SoundBuffer gun2ResetSound;
	static sf::Texture gun2BaseTexture;
	static sf::Texture gun2IconTexture;
	static std::vector<sf::Texture> gun2FireAnimationTexture;
	static std::vector<sf::Texture> gun2ResetAnimationTexture;

	static sf::SoundBuffer gun3ShutSound;
	static sf::SoundBuffer gun3ResetSound;
	static sf::Texture gun3BaseTexture;
	static sf::Texture gun3IconTexture;
	static std::vector<sf::Texture> gun3FireAnimationTexture;
	static std::vector<sf::Texture> gun3ResetAnimationTexture;

	static sf::SoundBuffer gun4ShutSound;
	static sf::SoundBuffer gun4ResetSound;
	static sf::Texture gun4BaseTexture;
	static sf::Texture gun4IconTexture;
	static std::vector<sf::Texture> gun4FireAnimationTexture;
	static std::vector<sf::Texture> gun4ResetAnimationTexture;

	static sf::SoundBuffer gun5ShutSound;
	static sf::SoundBuffer gun5ResetSound;
	static sf::Texture gun5BaseTexture;
	static sf::Texture gun5IconTexture;
	static std::vector<sf::Texture> gun5FireAnimationTexture;
	static std::vector<sf::Texture> gun5ResetAnimationTexture;

	static sf::SoundBuffer gun6ShutSound;
	static sf::SoundBuffer gun6ResetSound;
	static sf::Texture gun6BaseTexture;
	static sf::Texture gun6IconTexture;
	static std::vector<sf::Texture> gun6FireAnimationTexture;
	static std::vector<sf::Texture> gun6ResetAnimationTexture;

	static sf::Font UIFont;

	static void initResources();
};

#endif // !RESOURCES
