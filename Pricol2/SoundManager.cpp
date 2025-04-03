#include "SoundManager.h"

SoundManager::SoundManager() {}

std::vector<std::unique_ptr<sf::Sound>> SoundManager::sounds{};

void SoundManager::playSound(sf::SoundBuffer& buffer, float volume, sf::Vector2f pos, bool isLoop, bool isRelative)
{
	sounds.push_back(std::make_unique<sf::Sound>(buffer));
	sounds.back()->setLoop(isLoop);
	sounds.back()->setPosition({ pos.x, pos.y, 0.0f });
	sounds.back()->setRelativeToListener(isRelative);
	sounds.back()->setVolume(volume);
	sounds.back()->play();
}

void SoundManager::stopAllSound()
{
	for (int i = 0; i < sounds.size(); i++)
	{
		sounds[i]->stop();
	}
	sounds.clear();
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


