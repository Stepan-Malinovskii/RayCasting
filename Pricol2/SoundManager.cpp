#include "SoundManager.h"
#include "Randomizer.h"

SoundManager::SoundManager() {}

std::vector<std::unique_ptr<sf::Sound>> SoundManager::sounds{};

sf::Music SoundManager::music{};

void SoundManager::playSound(sf::SoundBuffer& buffer, float volume, bool isLoop)
{
	for (int i = 0; i < sounds.size(); i++)
	{
		if (sounds[i]->getBuffer() == &buffer)
		{
			sounds[i]->stop();
			sounds[i]->play();
			return;
		}	
	}

	sounds.push_back(std::make_unique<sf::Sound>(buffer));
	sounds.back()->setLoop(isLoop);
	sounds.back()->setVolume(volume);
	sounds.back()->play();
}

void SoundManager::playerMusic(MusicType type)
{
	std::string filePath = "Sound/background";

	if (type == Menu)
	{
		filePath += "1";
	}
	else if (type == Base)
	{
		filePath += "2";
	}
	else if (type == Level)
	{
		filePath += std::to_string(Random::intRandom(3, 9));
	}

	if (!music.openFromFile(filePath + ".ogg")) return;
	music.setLoop(true);
	music.play();
}

void SoundManager::stopAllSound()
{
	for (int i = 0; i < sounds.size(); i++)
	{
		sounds[i]->stop();
	}

	sounds.clear();
	music.stop();
}

void SoundManager::update()
{
	int i = 0;

	while (i < sounds.size())
	{
		if (sounds[i]->getStatus() == sf::Sound::Stopped)
		{
			sounds.erase(i + sounds.begin());
		}
		else
		{
			i++;
		}
	}
}


