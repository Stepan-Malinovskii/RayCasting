#pragma once
#ifndef SOUNDM
#define SOUNMD

#include <vector>
#include "Resources.h"
#include <SFML/System/Vector3.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class SoundManager
{
public:
	SoundManager();
	static void playSound(sf::SoundBuffer& buffer, float volume = 100, sf::Vector2f pos = {}, bool isLoop = false, bool isRelative = false);
	static void stopAllSound();
	static void update();	
private:
	static std::vector<std::unique_ptr<sf::Sound>> sounds;
};
#endif // !SOUNDM