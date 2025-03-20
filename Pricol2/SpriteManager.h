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
	SpriteManager(Map* _nowMap, Dialog* _dialogSys);
	~SpriteManager();
	void saveSprite();
	void update(float deltaTime);
	Player* resetMap(Map* newMap);
	Player* getPlayer();
	Npc getNpc(MapSprite* mSp);
	std::vector<std::shared_ptr<Sprite>>& getSprites();
private:
	int id;
	void deleteSprite(std::shared_ptr<Sprite> sp);
	void init();

	Dialog* diaologSys;
	std::vector<std::shared_ptr<Sprite>> sprites;
	std::unique_ptr<Player> player;
	Map* nowMap;
};

#endif // !SPMANAGER
