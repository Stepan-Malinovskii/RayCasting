#pragma once
#ifndef SPMANAGER
#define SPMANAGER

#include "Resources.h"
#include "Raycast.h"
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
	std::vector<std::shared_ptr<Sprite>>* getDeteachSprite();
private:
	int id;
	void killEnemy(Enemy* enem);
	EnemyState determineNewState(Enemy* enemy, float distance);
	void createSpriteFromMapSprite(MapSprite mapSprite);
	void createEnemy(MapSprite mapSprite, SpriteDef def);
	void createNpc(MapSprite mapSprite, SpriteDef );
	void createDefaultPlayer();
	void aiControler(float deltaTime);
	bool isCanAttack(Enemy* enemy, float deltaTime);
	void init();

	Dialog* dialogSys;
	Data* data;
	std::vector<std::shared_ptr<Sprite>>* allSprites;
	std::vector<Enemy*> enemys;
	std::unique_ptr<Player> player;
	Map* nowMap;
};

#endif // !SPMANAGER
