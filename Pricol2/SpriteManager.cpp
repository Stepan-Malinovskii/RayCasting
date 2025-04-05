#include "SpriteManager.h"

#define SQUARE(a) ((a) * (a))
#define GETDIST(a,b) (SQUARE(a.x - b.x) + SQUARE(a.y - b.y))

SpriteManager::SpriteManager(Map* _nowMap, Data* _data, Dialog* _dialogSys) : 
	nowMap{ _nowMap }, dialogSys{ _dialogSys }, data{ _data }, id { 1 }
{
	init();
}

void SpriteManager::init()
{
	id = 1;
	sprites.clear();
	npcs.clear();
	if (enemys.size() != 0)
	{
		std::shared_ptr<Sprite> sp;
		for (int i = 0; i < enemys.size(); i++)
		{
			if (enemys[i]->id == 0)
			{
				sp = enemys[i];
				break;
			}
		}
		enemys.clear();
		enemys.push_back(std::move(sp));
	}

	for (auto sp : nowMap->getMapSprites()) {
		createSpriteFromMapSprite(sp);
	}

	if (!player) {
		createDefaultPlayer();
	}

	setupBlockmapAndStates();
}

void SpriteManager::createSpriteFromMapSprite(MapSprite mapSprite)
{
	auto def = spriteDefs[mapSprite.spriteDefId];
	std::shared_ptr<Sprite> sprite;

	if (mapSprite.spriteDefId < ENEMY_COUNT) 
	{
		sprite = createEnemy(mapSprite, def);
	}
	else 
	{
		sprite = createNpc(mapSprite, def);
	}

	sprites.push_back(sprite.get());
	id++;
}

std::shared_ptr<Sprite> SpriteManager::createEnemy(MapSprite mapSprite, SpriteDef def)
{
	enemys.push_back(std::make_shared<Sprite>(def, mapSprite, id));

	if (mapSprite.spriteDefId == 0 && !player)
	{
		PlayerDef plDef = data->getPlayerData();
		enemys.back()->id = 0;
		enemys.back()->spDef.maxHealpoint = plDef.maxHp;
		enemys.back()->spMap.nowHealPoint = plDef.nowHp;
		player = std::make_unique<Player>(enemys.back().get(), plDef, nowMap);
	}

	return enemys.back();
}

std::shared_ptr<Sprite> SpriteManager::createNpc(MapSprite mapSprite, SpriteDef def)
{
	int npcIndex = mapSprite.spriteDefId - ENEMY_COUNT;
	auto npc = std::make_shared<Npc>(def, mapSprite, id, npcIndex, dialogSys);
	npcs.push_back(npc);
	return npc;
}

void SpriteManager::createDefaultPlayer()
{
	PlayerDef plDef = data->getPlayerData();
	auto def = spriteDefs[0];
	def.maxHealpoint = plDef.maxHp;

	MapSprite defaultPlayerSprite{
		0,                              
		{2.0f, 2.0f},                   
		90.0f,                          
		plDef.nowHp                     
	};

	enemys.push_back(std::make_shared<Sprite>(def, defaultPlayerSprite, 0));
	sprites.push_back(enemys.back().get());
	player = std::make_unique<Player>(enemys.back().get(), plDef, nowMap);
}

void SpriteManager::setupBlockmapAndStates()
{
	for (auto sprite : sprites)
	{
		if (sprite->spMap.nowHealPoint > 0.0f)
		{
			nowMap->setupBlockmap(sprite);
			sprite->changeState(Stay);
		}
		else
		{
			sprite->changeState(Dead);
		}
	}
}

Player* SpriteManager::resetMap(Map* newMap, std::pair<sf::Vector2f, sf::Vector2f> mapPos)
{
	nowMap = newMap;
	init();
	player->sprite->spMap.position = mapPos.first;
	player->setNemMap(newMap);
	return player.get();
}

Player* SpriteManager::getPlayer() { return player.get(); }

Npc* SpriteManager::getNpc(int id)
{
	auto npc = std::find_if(npcs.begin(), npcs.end(), 
		[id](std::shared_ptr<Npc> npc) {return npc->id == id;});
	return npc != npcs.end() ? npc->get() : nullptr;
}

std::vector<Sprite*> SpriteManager::getDeteachSprite() { return sprites;  }

void SpriteManager::update(float deltaTime)
{
	for (auto enemy : enemys)
	{
		if (enemy->spDef.texture > ENEMY_COUNT - 1) continue;
		if (enemy.get()->spDef.texture != -1)
		{
			float distance = GETDIST(enemy->spMap.position, player->sprite->spMap.position);

			if (distance > 30 && enemy->state < Killes)
			{
				enemy->changeState(Stay);
			}
			else if (distance < 20 && enemy->state < Killes)
			{
				enemy->changeState(Run);
			}
		}
	}

	for (auto enemy : enemys)
	{
		enemy->update(deltaTime);

		if (enemy->state == Killes)
		{
			enemy->changeState(Dead);
			player->money += Random::intRandom((int)(enemy->spDef.midleDrop * 0.8f), (int)(enemy->spDef.midleDrop * 1.2f));
			deleteSprite(enemy);
		}
	}
}

void SpriteManager::deleteSprite(std::shared_ptr<Sprite> sp)
{
	nowMap->deleteInBlockMap(sp.get());
}

SpriteManager::~SpriteManager() {}