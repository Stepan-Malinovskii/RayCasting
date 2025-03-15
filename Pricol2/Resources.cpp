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
std::array<sf::Texture, 7> Resources::gun1ResetAnimationTexture{};

sf::SoundBuffer Resources::gun2ShutSound{};
sf::SoundBuffer Resources::gun2ResetSound{};
sf::Texture Resources::gun2BaseTexture{};
std::array<sf::Texture, 3> Resources::gun2FireAnimationTexture{};
std::array<sf::Texture, 7> Resources::gun2ResetAnimationTexture{};


sf::SoundBuffer Resources::gun3ShutSound{};
sf::SoundBuffer Resources::gun3ResetSound{};
sf::Texture Resources::gun3BaseTexture{};
std::array<sf::Texture, 3> Resources::gun3FireAnimationTexture{};
std::array<sf::Texture, 17> Resources::gun3ResetAnimationTexture{};

sf::SoundBuffer Resources::gun4ShutSound{};
sf::SoundBuffer Resources::gun4ResetSound{};
sf::Texture Resources::gun4BaseTexture{};
std::array<sf::Texture, 3> Resources::gun4FireAnimationTexture{};
std::array<sf::Texture, 11> Resources::gun4ResetAnimationTexture{};

sf::SoundBuffer Resources::gun5ShutSound{};
sf::SoundBuffer Resources::gun5ResetSound{};
sf::Texture Resources::gun5BaseTexture{};
std::array<sf::Texture, 3> Resources::gun5FireAnimationTexture{};
std::array<sf::Texture, 13> Resources::gun5ResetAnimationTexture{};

sf::Font Resources::UIFont{};

void Resources::initResources()
{
	if (!textureImage.loadFromFile("Texture/wall_texture.png")) throw "TextureLoadError!";
	if (!textures.loadFromImage(Resources::textureImage)) throw "TextureLoadError!";
	if (!spritesTexture.loadFromFile("Texture/enemys.png")) throw "TextureLoadError!";
	if (!skyTextures.loadFromFile("Texture/sky_texture.png")) throw "TextureLoadError!";
	skyTextures.setRepeated(true);

	if (!gun1BaseTexture.loadFromFile("Texture/gun1BaseTexture.png")) throw "TextureLoadError!";
	if (!gun1FireAnimationTexture[0].loadFromFile("Texture/gun1FireTexture1.png")) throw "TextureLoadError!";
	if (!gun1FireAnimationTexture[1].loadFromFile("Texture/gun1FireTexture2.png")) throw "TextureLoadError!";
	if (!gun1FireAnimationTexture[2].loadFromFile("Texture/gun1FireTexture3.png")) throw "TextureLoadError!";
	if (!gun1FireAnimationTexture[3].loadFromFile("Texture/gun1FireTexture4.png")) throw "TextureLoadError!";
	if (!gun1ResetSound.loadFromFile("Sound/gun2ResetSound.mp3")) throw "TextureLoadError!";
	if (!gun1ResetAnimationTexture[0].loadFromFile("Texture/gun1ResetTexture1.png")) throw "TextureLoadError!";
	if (!gun1ResetAnimationTexture[1].loadFromFile("Texture/gun1ResetTexture2.png")) throw "TextureLoadError!";
	if (!gun1ResetAnimationTexture[2].loadFromFile("Texture/gun1ResetTexture3.png")) throw "TextureLoadError!";
	if (!gun1ResetAnimationTexture[3].loadFromFile("Texture/gun1ResetTexture4.png")) throw "TextureLoadError!";
	if (!gun1ResetAnimationTexture[4].loadFromFile("Texture/gun1ResetTexture5.png")) throw "TextureLoadError!";
	if (!gun1ResetAnimationTexture[5].loadFromFile("Texture/gun1ResetTexture6.png")) throw "TextureLoadError!";
	if (!gun1ResetAnimationTexture[6].loadFromFile("Texture/gun1ResetTexture7.png")) throw "TextureLoadError!";
	if (!gun1ShutSound.loadFromFile("Sound/gun1ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun1ResetSound.loadFromFile("Sound/gun1ResetSound.mp3")) throw "TextureLoadError!";
	if (!gun1CantShoutSound.loadFromFile("Sound/gun1CantShutSound.mp3")) throw "TextureLoadError";

	if (!gun2BaseTexture.loadFromFile("Texture/gun2BaseTexture.png")) throw "TextureLoadError!";
	if (!gun2FireAnimationTexture[0].loadFromFile("Texture/gun2FireTexture1.png")) throw "TextureLoadError!";
	if (!gun2FireAnimationTexture[1].loadFromFile("Texture/gun2FireTexture2.png")) throw "TextureLoadError!";
	if (!gun2FireAnimationTexture[2].loadFromFile("Texture/gun2FireTexture3.png")) throw "TextureLoadError!";
	if (!gun2ShutSound.loadFromFile("Sound/gun2ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun2ResetSound.loadFromFile("Sound/gun2ResetSound.mp3")) throw "TextureLoadError!";
	if (!gun2ResetAnimationTexture[0].loadFromFile("Texture/gun2ResetTexture1.png")) throw "TextureLoadError!";
	if (!gun2ResetAnimationTexture[1].loadFromFile("Texture/gun2ResetTexture2.png")) throw "TextureLoadError!";
	if (!gun2ResetAnimationTexture[2].loadFromFile("Texture/gun2ResetTexture3.png")) throw "TextureLoadError!";
	if (!gun2ResetAnimationTexture[3].loadFromFile("Texture/gun2ResetTexture4.png")) throw "TextureLoadError!";
	if (!gun2ResetAnimationTexture[4].loadFromFile("Texture/gun2ResetTexture5.png")) throw "TextureLoadError!";
	if (!gun2ResetAnimationTexture[5].loadFromFile("Texture/gun2ResetTexture6.png")) throw "TextureLoadError!";
	if (!gun2ResetAnimationTexture[6].loadFromFile("Texture/gun2ResetTexture7.png")) throw "TextureLoadError!";

	if (!gun3BaseTexture.loadFromFile("Texture/gun3BaseTexture.png")) throw "TextureLoadError!";
	if (!gun3FireAnimationTexture[0].loadFromFile("Texture/gun3FireTexture1.png")) throw "TextureLoadError!";
	if (!gun3FireAnimationTexture[1].loadFromFile("Texture/gun3FireTexture2.png")) throw "TextureLoadError!";
	if (!gun3FireAnimationTexture[2].loadFromFile("Texture/gun3FireTexture3.png")) throw "TextureLoadError!";
	if (!gun3ShutSound.loadFromFile("Sound/gun3ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun3ResetSound.loadFromFile("Sound/gun3ResetSound.mp3")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[0].loadFromFile("Texture/gun3ResetTexture1.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[1].loadFromFile("Texture/gun3ResetTexture2.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[2].loadFromFile("Texture/gun3ResetTexture3.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[3].loadFromFile("Texture/gun3ResetTexture4.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[4].loadFromFile("Texture/gun3ResetTexture5.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[5].loadFromFile("Texture/gun3ResetTexture6.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[6].loadFromFile("Texture/gun3ResetTexture7.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[7].loadFromFile("Texture/gun3ResetTexture8.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[8].loadFromFile("Texture/gun3ResetTexture9.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[9].loadFromFile("Texture/gun3ResetTexture10.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[10].loadFromFile("Texture/gun3ResetTexture11.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[11].loadFromFile("Texture/gun3ResetTexture12.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[12].loadFromFile("Texture/gun3ResetTexture13.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[13].loadFromFile("Texture/gun3ResetTexture14.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[14].loadFromFile("Texture/gun3ResetTexture15.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[15].loadFromFile("Texture/gun3ResetTexture16.png")) throw "TextureLoadError!";
	if (!gun3ResetAnimationTexture[16].loadFromFile("Texture/gun3ResetTexture17.png")) throw "TextureLoadError!";

	if (!gun4BaseTexture.loadFromFile("Texture/gun4BaseTexture.png")) throw "TextureLoadError!";
	if (!gun4ShutSound.loadFromFile("Sound/gun4ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun4ResetSound.loadFromFile("Sound/gun4ResetSound.mp3")) throw "TextureLoadError!";
	if (!gun4FireAnimationTexture[0].loadFromFile("Texture/gun4FireTexture1.png")) throw "TextureLoadError!";
	if (!gun4FireAnimationTexture[1].loadFromFile("Texture/gun4FireTexture2.png")) throw "TextureLoadError!";
	if (!gun4FireAnimationTexture[2].loadFromFile("Texture/gun4FireTexture3.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[0].loadFromFile("Texture/gun4ResetTexture1.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[1].loadFromFile("Texture/gun4ResetTexture2.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[2].loadFromFile("Texture/gun4ResetTexture3.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[3].loadFromFile("Texture/gun4ResetTexture4.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[4].loadFromFile("Texture/gun4ResetTexture5.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[5].loadFromFile("Texture/gun4ResetTexture6.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[6].loadFromFile("Texture/gun4ResetTexture7.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[7].loadFromFile("Texture/gun4ResetTexture8.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[8].loadFromFile("Texture/gun4ResetTexture9.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[9].loadFromFile("Texture/gun4ResetTexture10.png")) throw "TextureLoadError!";
	if (!gun4ResetAnimationTexture[10].loadFromFile("Texture/gun4ResetTexture11.png")) throw "TextureLoadError!";
	
	if (!gun5BaseTexture.loadFromFile("Texture/gun5BaseTexture.png")) throw "TextureLoadError!";
	if (!gun5ShutSound.loadFromFile("Sound/gun5ShutSound.mp3")) throw "TextureLoadError!";
	if (!gun5ResetSound.loadFromFile("Sound/gun5ResetSound.mp3")) throw "TextureLoadError!";
	if (!gun5FireAnimationTexture[0].loadFromFile("Texture/gun5FireTexture1.png")) throw "TextureLoadError!";
	if (!gun5FireAnimationTexture[1].loadFromFile("Texture/gun5FireTexture2.png")) throw "TextureLoadError!";
	if (!gun5FireAnimationTexture[2].loadFromFile("Texture/gun5FireTexture3.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[0].loadFromFile("Texture/gun5ResetTexture1.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[1].loadFromFile("Texture/gun5ResetTexture2.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[2].loadFromFile("Texture/gun5ResetTexture3.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[3].loadFromFile("Texture/gun5ResetTexture4.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[4].loadFromFile("Texture/gun5ResetTexture5.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[5].loadFromFile("Texture/gun5ResetTexture6.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[6].loadFromFile("Texture/gun5ResetTexture7.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[7].loadFromFile("Texture/gun5ResetTexture8.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[8].loadFromFile("Texture/gun5ResetTexture9.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[9].loadFromFile("Texture/gun5ResetTexture10.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[10].loadFromFile("Texture/gun5ResetTexture11.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[11].loadFromFile("Texture/gun5ResetTexture12.png")) throw "TextureLoadError!";
	if (!gun5ResetAnimationTexture[12].loadFromFile("Texture/gun5ResetTexture13.png")) throw "TextureLoadError!";

	if (!UIFont.loadFromFile("Texture/Pshek_KY2.ttf")) throw "TextureLoadError!";
}