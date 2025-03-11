#pragma once
#ifndef SPMANAGER
#define SPMANAGER

#include "Resources.h"
#include "Sprite.h"
#include "Player.h"
#include "Map.h"

class SpriteManager
{
public:
	SpriteManager(Map* nowMap);
	SpriteManager() = default;
	~SpriteManager();
	void update(float deltaTime);
	Player* resetMap(Map* newMap);
	Player* getPlayer();
	std::vector<std::shared_ptr<Sprite>>& getSprites();
private:
	void deleteSprite(std::shared_ptr<Sprite> sp);
	void init();

	std::vector<std::shared_ptr<Sprite>> sprites;
	std::unique_ptr<Player> player;
	Map* nowMap;
};

#endif // !SPMANAGER
