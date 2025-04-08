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
	for (int i = 0; i < allSprite.size(); i++)
	{
		allSprite[i].reset();
	}
	allSprite.clear();
	sprites.clear();
	dead.clear();
	enemys.clear();
	player.reset();

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

	mapSprite.spriteDefId < ENEMY_COUNT ? createEnemy(mapSprite, def) : createNpc(mapSprite, def);
	
	id++;
}

std::shared_ptr<Sprite> SpriteManager::createEnemy(MapSprite mapSprite, SpriteDef def)
{
	allSprite.push_back(std::make_shared<Sprite>(def, mapSprite, id));

	if (mapSprite.spriteDefId == 0)
	{
		PlayerDef plDef = data->getPlayerData();
		allSprite.back()->id = 0;
		allSprite.back()->spDef.maxHealpoint = plDef.maxHp;
		allSprite.back()->spMap.nowHealPoint = plDef.nowHp;
		player = std::make_unique<Player>(allSprite.back().get(), plDef, nowMap);
		player->patrons = plDef.countpantrons;
	}

	return allSprite.back();
}

std::shared_ptr<Sprite> SpriteManager::createNpc(MapSprite mapSprite, SpriteDef def)
{
	allSprite.push_back(std::make_shared<Npc>(def, mapSprite, id, mapSprite.spriteDefId - ENEMY_COUNT, dialogSys));
	return allSprite.back();
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

	allSprite.push_back(std::make_shared<Sprite>(def, defaultPlayerSprite, 0));
	sprites.push_back(allSprite.back().get());
	player = std::make_unique<Player>(allSprite.back().get(), plDef, nowMap);
	player->patrons = plDef.countpantrons;
}

void SpriteManager::setupBlockmapAndStates()
{
	for (auto sprite : allSprite)
	{
		if (sprite->spMap.nowHealPoint <= 0.0f)
		{
			sprite->changeState(Dead);
			dead.push_back(sprite.get());
		}
		else 
		{
			nowMap->setupBlockmap(sprite.get());
			sprite->changeState(Stay);

			if (sprite->spDef.texture < ENEMY_COUNT - 1)
			{
				enemys.push_back(sprite.get());
			}
		}

		sprites.push_back(sprite.get());
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
	auto npc = std::find_if(allSprite.begin(), allSprite.end(), 
		[id](std::shared_ptr<Sprite> sp) {return sp->id == id;});
	return npc != allSprite.end() ? dynamic_cast<Npc*>(npc->get()) : nullptr;
}

std::vector<Sprite*> SpriteManager::getDeteachSprite() { return sprites;  }

void SpriteManager::update(float deltaTime)
{
	for (auto enemy : enemys)
	{
		enemy->update(deltaTime);

		if (enemy->state == Killes)
		{
			killEnemy(enemy);
		}
	}

	for (auto enemy : enemys)
	{
		if (enemy->spDef.texture == -1) continue;	
		
		float distance = GETDIST(enemy->spMap.position, player->sprite->spMap.position);

		float angle = enemy->spMap.angle * PI / 180.0f;
		sf::Vector2f dir{ cos(angle), sin(angle) };
		sf::Vector2f toPlayerDir = player->sprite->spMap.position
			- enemy->spMap.position;

		if (distance < enemy->spDef.atackDist)
		{
			enemy->changeState(Atack);
			enemy->spMap.angle = std::atan2(toPlayerDir.y, toPlayerDir.x) * 180.0f / PI;
		}
		else if (distance < TRIGER_DIST_MIN)
		{
			enemy->changeState(Run);
			enemy->spMap.angle = std::atan2(toPlayerDir.y, toPlayerDir.x) * 180.0f / PI;
			enemy->move(nowMap, enemy->spDef.speed * deltaTime * dir);
		}
		else if (distance > TRIGER_DIST_MAX)
		{
			enemy->changeState(Stay);
		}
	}
}

void SpriteManager::killEnemy(Sprite* sp)
{
	sp->changeState(Dead);
	player->money += Random::intRandom((int)(sp->spDef.midleDrop * 0.8f), (int)(sp->spDef.midleDrop * 1.2f));
	nowMap->deleteInBlockMap(sp);

	dead.push_back(sp);

	for (int i = 0; i < enemys.size(); i++)
	{
		if (enemys[i]->id == sp->id)
		{
			enemys.erase(enemys.begin() + i);
			break;
		}
	}
}

SpriteManager::~SpriteManager() {}