#pragma once
#ifndef SPMANAGER
#define SPMANAGER

#include "Resources.h"
#include "Raycast.h"
#include "Sprite.h"
#include "Player.h"
#include "Map.h"
#include "DataBase.h"
#include "MapManager.h"

class SpriteManager
{
public:
	SpriteManager(Map* _nowMap, UIManager* uiManager, ItemManager* _itemManager);
	~SpriteManager();
	void update(float deltaTime);
	void resetMap(Map* newMap, sf::Vector2f playerPos);
	Player* getPlayer();
	Npc* getNpc(int id);
	std::vector<std::shared_ptr<Sprite>>* getDeteachSprite();
private:
	int id;
	void killEnemy(Enemy* enem);
	EnemyState determineNewState(Enemy* enemy, float distance);
	void createSpriteFromMapSprite(MapSprite mapSprite);
	void createEnemy(MapSprite mapSprite, SpriteDef def);
	void createPlayer(MapSprite mapSprite, SpriteDef def, PlayerDef plDef);
	void createNpc(MapSprite mapSprite, SpriteDef );
	void createDefaultPlayer(PlayerDef plDef);
	void aiControler(float deltaTime);
	bool isEnemyHit(Enemy* enemy);
	void init();

	UIManager* uiManager;
	ItemManager* itemManager;
	std::vector<std::shared_ptr<Sprite>>* allSprites;
	std::vector<Enemy*> enemys;
	std::unique_ptr<Player> player;
	Map* nowMap;
};

#endif // !SPMANAGER
