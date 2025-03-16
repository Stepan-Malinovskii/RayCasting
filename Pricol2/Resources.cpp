#include "Resources.h"

sf::Texture Resources::textures{};
sf::Image Resources::textureImage{};
sf::Texture Resources::spritesTexture{};
sf::Texture Resources::skyTextures{};

sf::SoundBuffer Resources::gun1ShutSound{};
sf::SoundBuffer Resources::gun1ResetSound{};
sf::SoundBuffer Resources::gun1CantShoutSound{};
sf::Texture Resources::gun1BaseTexture{};
sf::Texture Resources::gun1IconTexture{};
std::vector<sf::Texture> Resources::gun1FireAnimationTexture{};
std::vector<sf::Texture> Resources::gun1ResetAnimationTexture{};

sf::SoundBuffer Resources::gun2ShutSound{};
sf::SoundBuffer Resources::gun2ResetSound{};
sf::Texture Resources::gun2BaseTexture{};
sf::Texture Resources::gun2IconTexture{};
std::vector<sf::Texture> Resources::gun2FireAnimationTexture{};
std::vector<sf::Texture> Resources::gun2ResetAnimationTexture{};

sf::SoundBuffer Resources::gun3ShutSound{};
sf::SoundBuffer Resources::gun3ResetSound{};
sf::Texture Resources::gun3BaseTexture{};
sf::Texture Resources::gun3IconTexture{};
std::vector<sf::Texture> Resources::gun3FireAnimationTexture{};
std::vector<sf::Texture> Resources::gun3ResetAnimationTexture{};

sf::SoundBuffer Resources::gun4ShutSound{};
sf::SoundBuffer Resources::gun4ResetSound{};
sf::Texture Resources::gun4BaseTexture{};
sf::Texture Resources::gun4IconTexture{};
std::vector<sf::Texture> Resources::gun4FireAnimationTexture{};
std::vector<sf::Texture> Resources::gun4ResetAnimationTexture{};

sf::SoundBuffer Resources::gun5ShutSound{};
sf::SoundBuffer Resources::gun5ResetSound{};
sf::Texture Resources::gun5BaseTexture{};
sf::Texture Resources::gun5IconTexture{};
std::vector<sf::Texture> Resources::gun5FireAnimationTexture{};
std::vector<sf::Texture> Resources::gun5ResetAnimationTexture{};

sf::SoundBuffer Resources::gun6ShutSound{};
sf::SoundBuffer Resources::gun6ResetSound{};
sf::Texture Resources::gun6BaseTexture{};
sf::Texture Resources::gun6IconTexture{};
std::vector<sf::Texture> Resources::gun6FireAnimationTexture{};
std::vector<sf::Texture> Resources::gun6ResetAnimationTexture{};

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

void Resources::initResources()
{
	if (!textureImage.loadFromFile("Texture/wall_texture.png")) throw "TextureLoadError!";
	if (!textures.loadFromImage(Resources::textureImage)) throw "TextureLoadError!";
	if (!spritesTexture.loadFromFile("Texture/enemys.png")) throw "TextureLoadError!";
	if (!skyTextures.loadFromFile("Texture/sky_texture.png")) throw "TextureLoadError!";
	skyTextures.setRepeated(true);

	if (!gun1BaseTexture.loadFromFile("Texture/gun1BaseTexture.png")) throw "TextureLoadError!";
	if (!gun1IconTexture.loadFromFile("Texture/gun1IconTexture.png")) throw "TextureLoadError!";
	loadFor("Texture/gun1FireTexture", &gun1FireAnimationTexture);
	loadFor("Texture/gun1ResetTexture", &gun1ResetAnimationTexture);
	if (!gun1ShutSound.loadFromFile("Sound/gun1ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun1ResetSound.loadFromFile("Sound/gun1ResetSound.mp3")) throw "TextureLoadError!";
	if (!gun1CantShoutSound.loadFromFile("Sound/gun1CantShutSound.mp3")) throw "TextureLoadError";

	if (!gun2BaseTexture.loadFromFile("Texture/gun2BaseTexture.png")) throw "TextureLoadError!";
	if (!gun2IconTexture.loadFromFile("Texture/gun2IconTexture.png")) throw "TextureLoadError!";
	loadFor("Texture/gun2FireTexture", &gun2FireAnimationTexture);
	loadFor("Texture/gun2ResetTexture", &gun2ResetAnimationTexture);
	if (!gun2ShutSound.loadFromFile("Sound/gun2ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun2ResetSound.loadFromFile("Sound/gun2ResetSound.mp3")) throw "TextureLoadError!";

	if (!gun3BaseTexture.loadFromFile("Texture/gun3BaseTexture.png")) throw "TextureLoadError!";
	if (!gun3IconTexture.loadFromFile("Texture/gun3IconTexture.png")) throw "TextureLoadError!";
	loadFor("Texture/gun3FireTexture", &gun3FireAnimationTexture);
	loadFor("Texture/gun3ResetTexture", &gun3ResetAnimationTexture);
	if (!gun3ShutSound.loadFromFile("Sound/gun3ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun3ResetSound.loadFromFile("Sound/gun3ResetSound.mp3")) throw "TextureLoadError!";

	if (!gun4BaseTexture.loadFromFile("Texture/gun4BaseTexture.png")) throw "TextureLoadError!";
	if (!gun4IconTexture.loadFromFile("Texture/gun4IconTexture.png")) throw "TextureLoadError!";
	loadFor("Texture/gun4FireTexture", &gun4FireAnimationTexture);
	loadFor("Texture/gun4ResetTexture", &gun4ResetAnimationTexture);
	if (!gun4ShutSound.loadFromFile("Sound/gun4ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun4ResetSound.loadFromFile("Sound/gun4ResetSound.mp3")) throw "TextureLoadError!";

	if (!gun5BaseTexture.loadFromFile("Texture/gun5BaseTexture.png")) throw "TextureLoadError!";
	if (!gun5IconTexture.loadFromFile("Texture/gun5IconTexture.png")) throw "TextureLoadError!";
	loadFor("Texture/gun5FireTexture", &gun5FireAnimationTexture);
	loadFor("Texture/gun5ResetTexture", &gun5ResetAnimationTexture);
	if (!gun5ShutSound.loadFromFile("Sound/gun5ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun5ResetSound.loadFromFile("Sound/gun5ResetSound.mp3")) throw "TextureLoadError!";

	if (!gun6BaseTexture.loadFromFile("Texture/gun6BaseTexture.png")) throw "TextureLoadError!";
	if (!gun6IconTexture.loadFromFile("Texture/gun6IconTexture.png")) throw "TextureLoadError!";
	loadFor("Texture/gun6FireTexture", &gun6FireAnimationTexture);
	loadFor("Texture/gun6ResetTexture", &gun6ResetAnimationTexture);
	if (!gun6ShutSound.loadFromFile("Sound/gun6ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun6ResetSound.loadFromFile("Sound/gun6ResetSound.mp3")) throw "TextureLoadError!";

	if (!UIFont.loadFromFile("Texture/Pshek_KY2.ttf")) throw "TextureLoadError!";
}