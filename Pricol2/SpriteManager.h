#pragma once
#ifndef SPMANAGER
#define SPMANAGER

#include "Resources.h"
#include "Sprite.h"
#include "Player.h"
#include "Map.h"
#include "DataBase.h"

class SpriteManager
{
public:
	SpriteManager(Map* _nowMap, Data* _data, Dialog* _dialogSys);
	~SpriteManager();
	void update(float deltaTime);
	Player* resetMap(Map* newMap);
	Player* getPlayer();
	Npc getNpc(MapSprite* mSp);
	std::vector<std::shared_ptr<Sprite>> getSprites();
private:
	int id;
	void deleteSprite(std::shared_ptr<Sprite> sp);
	void init();

	Dialog* dialogSys;
	Data* data;
	std::vector<std::shared_ptr<Sprite>> sprites;
	std::unique_ptr<Player> player;
	Map* nowMap;
};

#endif // !SPMANAGER
