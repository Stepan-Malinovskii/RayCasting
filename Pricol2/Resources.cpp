#include "Resources.h"

sf::Texture Resources::textures{};

sf::Image Resources::textureImage{};

sf::Texture Resources::spritesTexture{};

sf::Texture Resources::skyTextures{};

sf::SoundBuffer Resources::gun1ShutSound{};

sf::SoundBuffer Resources::gun1ResetSound{};

sf::SoundBuffer Resources::gun1CantShoutSound{};

sf::Texture Resources::gun1BaseTexture{};

std::array<sf::Texture, 4> Resources::gun1FireAnimationTexture{};

sf::SoundBuffer Resources::gun2ShutSound{};

sf::SoundBuffer Resources::gun2ResetSound{};

sf::SoundBuffer Resources::gun2CantShoutSound{};

sf::Texture Resources::gun2BaseTexture{};

std::array<sf::Texture, 4> Resources::gun2FireAnimationTexture{};

sf::Font Resources::UIFont{};

void Resources::initResources()
{
	if (!textureImage.loadFromFile("Texture/wall_texture.png")) throw "TextureLoadError!";
	if (!textures.loadFromImage(Resources::textureImage)) throw "TextureLoadError!";
	if (!spritesTexture.loadFromFile("Texture/enemys1.png")) throw "TextureLoadError!";
	if (!skyTextures.loadFromFile("Texture/sky_texture.png")) throw "TextureLoadError!";
	skyTextures.setRepeated(true);

	if (!gun1BaseTexture.loadFromFile("Texture/weapon.png")) throw "TextureLoadError!";
	if (!gun1FireAnimationTexture[0].loadFromFile("Texture/weapon_fire10.png")) throw "TextureLoadError!";
	if (!gun1FireAnimationTexture[1].loadFromFile("Texture/weapon_fire11.png")) throw "TextureLoadError!";
	if (!gun1FireAnimationTexture[2].loadFromFile("Texture/weapon_fire12.png")) throw "TextureLoadError!";
	if (!gun1FireAnimationTexture[3].loadFromFile("Texture/weapon_fire13.png")) throw "TextureLoadError!";
	if (!gun1ShutSound.loadFromFile("Sound/gun1ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun1ResetSound.loadFromFile("Sound/gun1ResetSound.mp3")) throw "TextureLoadError!";
	if (!gun1CantShoutSound.loadFromFile("Sound/gun1CantShutSound.mp3")) throw "TextureLoadError";
	if (!gun2BaseTexture.loadFromFile("Texture/gun2BaseTexture.png")) throw "TextureLoadError!";
	if (!gun2FireAnimationTexture[0].loadFromFile("Texture/gun2FireTexture1.png")) throw "TextureLoadError!";
	if (!gun2FireAnimationTexture[1].loadFromFile("Texture/gun2FireTexture2.png")) throw "TextureLoadError!";
	if (!gun2FireAnimationTexture[2].loadFromFile("Texture/gun2FireTexture3.png")) throw "TextureLoadError!";
	if (!gun2FireAnimationTexture[3].loadFromFile("Texture/gun2FireTexture4.png")) throw "TextureLoadError!";
	if (!gun2ShutSound.loadFromFile("Sound/gun2ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun2ResetSound.loadFromFile("Sound/gun2ResetSound.mp3")) throw "TextureLoadError!";
	if (!gun2CantShoutSound.loadFromFile("Sound/gun1CantShutSound.mp3")) throw "TextureLoadError!";
	if (!UIFont.loadFromFile("Texture/Pshek_KY2.ttf")) throw "TextureLoadError!";
}