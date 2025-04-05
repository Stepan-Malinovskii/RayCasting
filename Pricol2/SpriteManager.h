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
	Player* resetMap(Map* newMap, std::pair<sf::Vector2f, sf::Vector2f> mapPos);
	Player* getPlayer();
	Npc* getNpc(int id);
	std::vector<Sprite*> getDeteachSprite();
private:
	int id;
	void deleteSprite(std::shared_ptr<Sprite> sp);
	void createSpriteFromMapSprite(MapSprite mapSprite);
	std::shared_ptr<Sprite> createEnemy(MapSprite mapSprite, SpriteDef def);
	std::shared_ptr<Sprite> createNpc(MapSprite mapSprite, SpriteDef );
	void createDefaultPlayer();
	void setupBlockmapAndStates();
	void init();

	Dialog* dialogSys;
	Data* data;
	std::vector<std::shared_ptr<Sprite>> enemys;
	std::vector<std::shared_ptr<Npc>> npcs;
	std::vector<Sprite*> sprites;
	std::vector<std::pair<float, std::shared_ptr<Sprite>>> to_dead;
	std::unique_ptr<Player> player;
	Map* nowMap;
};

#endif // !SPMANAGER
