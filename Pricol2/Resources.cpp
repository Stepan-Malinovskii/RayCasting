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