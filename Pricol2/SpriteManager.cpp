#include "SpriteManager.h"

#define SQUARE(a) ((a) * (a))
#define GETDIST(a,b) (SQUARE(a.x - b.x) + SQUARE(a.y - b.y))

SpriteManager::SpriteManager(Map* _nowMap, Data* _data, Dialog* _dialogSys) : 
	nowMap{ _nowMap }, dialogSys{ _dialogSys }, data{ _data }, id { 1 }
{
	allSprites = new std::vector<std::shared_ptr<Sprite>>();
	init();
}

void SpriteManager::init()
{
	id = 1;
	for (int i = 0; i < allSprites->size(); i++)
	{
		(*allSprites)[i].reset();
	}
	allSprites->clear();
	enemys.clear();
	player.reset();

	for (auto sp : nowMap->getMapSprites()) {
		createSpriteFromMapSprite(sp);
	}

	if (!player) {
		createDefaultPlayer();
	}
}

void SpriteManager::createSpriteFromMapSprite(MapSprite mapSprite)
{
	auto def = spriteDefs[mapSprite.spriteDefId];

	mapSprite.spriteDefId < ENEMY_COUNT ? createEnemy(mapSprite, def) : createNpc(mapSprite, def);
	
	id++;
}

void SpriteManager::createEnemy(MapSprite mapSprite, SpriteDef def)
{
	auto enemy = std::make_shared<Enemy>(def, mapSprite, enemyDef[mapSprite.spriteDefId], id);

	if (mapSprite.spriteDefId == 0)
	{
		PlayerDef plDef = data->getPlayerData();
		enemy->id = 0;
		enemy->enemyDef.maxHealpoint = plDef.maxHp;
		enemy->spMap.nowHealPoint = plDef.nowHp;
		player = std::make_unique<Player>(enemy.get(), plDef, nowMap);
		player->patrons = plDef.countpantrons;
		nowMap->setupBlockmap(enemy.get());
	}
	else
	{
		if (enemy->spMap.nowHealPoint <= 0.0f)
		{
			enemy->changeState(Dead);
		}
		else
		{
			enemy->changeState(Stay);
			enemys.push_back(enemy.get());
			nowMap->setupBlockmap(enemy.get());
		}
	}

	allSprites->push_back(std::move(enemy));
}

void SpriteManager::createNpc(MapSprite mapSprite, SpriteDef def)
{
	allSprites->push_back(std::make_shared<Npc>(Npc(def, mapSprite, id, mapSprite.spriteDefId - ENEMY_COUNT, dialogSys)));
	nowMap->setupBlockmap(allSprites->back().get());
}

void SpriteManager::createDefaultPlayer()
{
	PlayerDef plDef = data->getPlayerData();
	auto def = spriteDefs[0];
	auto enDef = enemyDef[0];
	enDef.maxHealpoint = plDef.maxHp;

	MapSprite defaultPlayerSprite{
		0,                              
		{2.0f, 2.0f},                   
		90.0f,                          
		plDef.nowHp                     
	};

	auto enemy = std::make_shared<Enemy>(def, defaultPlayerSprite, enDef, 0);
	nowMap->setupBlockmap(enemy.get());

	player = std::make_unique<Player>(enemy.get(), plDef, nowMap);
	player->patrons = plDef.countpantrons;

	allSprites->push_back(std::move(enemy));
}

Player* SpriteManager::resetMap(Map* newMap, std::pair<sf::Vector2f, sf::Vector2f> mapPos)
{
	nowMap = newMap;
	init();
	player->enemy->spMap.position = mapPos.first;
	player->setNemMap(newMap);
	return player.get();
}

Player* SpriteManager::getPlayer() { return player.get(); }

Npc* SpriteManager::getNpc(int id)
{
	auto npc = std::find_if(allSprites->begin(), allSprites->end(), 
		[id](std::shared_ptr<Sprite> sp) {return sp->id == id;});
	return npc != allSprites->end() ? dynamic_cast<Npc*>(npc->get()) : nullptr;
}

std::vector<std::shared_ptr<Sprite>>* SpriteManager::getDeteachSprite() { return allSprites;  }

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

	aiControler(deltaTime);
}

void SpriteManager::aiControler(float deltaTime)
{
	for (auto enemy : enemys)
	{
		if (enemy->isAtack && enemy->animr.get() == 0)
		{
			enemy->isAtack = false;
			player->takeDamage(enemy->enemyDef.damage);
		}

		float distance = GETDIST(enemy->spMap.position, player->enemy->spMap.position);

		sf::Vector2f toPlayerDir = player->enemy->spMap.position - enemy->spMap.position;

		float angle = enemy->spMap.angle * PI / 180.0f;
		sf::Vector2f dir{ cos(angle), sin(angle) };

		auto newState = determineNewState(enemy, distance);

		if (newState == Run)
		{
			if (enemy->canChangeState)
			{
				if (Random::bitRandom() > 0.7f) enemy->spMap.angle = std::atan2(toPlayerDir.y, toPlayerDir.x) * 180.0f / PI;
				enemy->move(nowMap, enemy->enemyDef.speed * deltaTime * dir);
				enemy->changeState(newState);
			}
		}
		else if (newState == Attack)
		{
			if (enemy->changeState(newState))
			{
				if (isCanAttack(enemy, deltaTime))
				{
					enemy->isAtack = true;
				}
				else
				{
					if (Random::bitRandom() > 0.3f) enemy->spMap.angle = std::atan2(toPlayerDir.y, toPlayerDir.x) * 180.0f / PI;
				}
			}
		}
		else if (newState == Stay)
		{
			enemy->changeState(newState);
		}
	}
}

EnemyState SpriteManager::determineNewState(Enemy* enemy, float distance)
{
	if (distance < enemy->enemyDef.atackDist)
	{
		return Attack;
	}
	else if (distance < TRIGER_DIST_MIN)
	{
		return Run;
	}
	else if (distance > TRIGER_DIST_MAX)
	{
		return Stay;
	}
}

bool SpriteManager::isCanAttack(Enemy* enemy, float deltaTime)
{
	if (!enemy->isCanAttack) return false;

	enemy->isCanAttack = false;

	float angle = enemy->spMap.angle * PI / 180.0f;
	sf::Vector2f dir{ cos(angle), sin(angle) };

	RayHit hit = raycast(nowMap, enemy->spMap.position, dir, true, enemy, enemy->enemyDef.atackDist);
	if (hit.sprite)
	{
		if (hit.sprite == dynamic_cast<Sprite*>(player->enemy))
		{
			return true;
		}
	}
	return false;
}

void SpriteManager::killEnemy(Enemy* enem)
{
	enem->changeState(Dead);
	player->money += Random::intRandom((int)(enem->enemyDef.midleDrop * 0.8f), (int)(enem->enemyDef.midleDrop * 1.2f));
	nowMap->deleteInBlockMap(enem);

	for (int i = 0; i < enemys.size(); i++)
	{
		if (enemys[i]->id == enem->id)
		{
			enemys.erase(enemys.begin() + i);
			break;
		}
	}
}

SpriteManager::~SpriteManager()
{
	delete allSprites;
}