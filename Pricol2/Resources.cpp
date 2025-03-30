#include "Resources.h"

sf::Texture Resources::textures{};
sf::Texture Resources::dialogBackround{};
sf::Image Resources::textureImage{};
sf::Texture Resources::spriteIcon{};
std::vector<sf::Texture> Resources::spritesTextures{};
sf::Texture Resources::skyTextures{};

sf::Texture Resources::itembleIcon{};
sf::SoundBuffer Resources::gunCantShoutSound{};
std::vector<std::vector<sf::Texture>> Resources::gunsResetAnim = std::vector<std::vector<sf::Texture>>(8);
std::vector<std::vector<sf::Texture>> Resources::gunsFireAnim = std::vector<std::vector<sf::Texture>>(8);
std::vector<sf::Texture> Resources::gunsBaseText = std::vector<sf::Texture>(8);
std::vector<sf::SoundBuffer> Resources::gunsShutSound = std::vector<sf::SoundBuffer>(8);
std::vector<sf::SoundBuffer> Resources::gunsResetSound = std::vector<sf::SoundBuffer>(8);

sf::Font Resources::UIFont{};

void loadFor(std::string baseName, std::vector<sf::Texture>* data)
{
	int i = 1;
	while (true)
	{
		std::string tryName = baseName + std::to_string(i) + ".png";
		sf::Texture text;
		if (!text.loadFromFile(tryName)) break;
		i++;
		data->push_back(text);
	}
}

void Resources::loadGun(std::string baseName, int index)
{
	std::string shutName = "Texture/" + baseName + "FireTexture";
	loadFor(shutName, &gunsFireAnim[index]);
	std::string resetName = "Texture/" + baseName + "ResetTexture";
	loadFor(resetName, &gunsResetAnim[index]);
	std::string baseTextName = "Texture/" + baseName + "BaseTexture.png";
	gunsBaseText[index].loadFromFile(baseTextName);
	std::string resetSoundName = "Sound/" + baseName + "ResetSound.mp3";
	gunsResetSound[index].loadFromFile(resetSoundName);
	std::string shutSoundName = "Sound/" + baseName + "ShutSound.mp3";
	gunsShutSound[index].loadFromFile(shutSoundName);
}

void Resources::initResources()
{
	if (!textureImage.loadFromFile("Texture/wall_texture.png")) throw "TextureLoadError!";
	if (!dialogBackround.loadFromFile("Texture/dialogBackground.png")) throw "TextureLoadError!";
	if (!textures.loadFromFile("Texture/wall_texture.png")) throw "TextureLoadError!";
	if (!skyTextures.loadFromFile("Texture/sky_texture.png")) throw "TextureLoadError!";
	if (!spriteIcon.loadFromFile("Texture/enemysIcon.png")) throw "TextureLoadError!";
	loadFor("Texture/enemy", &spritesTextures);
	skyTextures.setRepeated(true);

	if (!itembleIcon.loadFromFile("Texture/itemIcon.png")) throw "TextureLoadError";
	if (!gunCantShoutSound.loadFromFile("Sound/gunCantShutSound.mp3")) throw "TextureLoadError";

	for (int i = 0; i < 8; i++)
	{
		loadGun("gun" + std::to_string(i), i);
	}

	if (!UIFont.loadFromFile("Texture/dehinted-HandveticaNeue-Regular.ttf")) throw "TextureLoadError!";
}