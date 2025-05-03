#include "SpriteManager.h"

#define SQUARE(a) ((a) * (a))
#define GETDIST(a,b) (SQUARE(a.x - b.x) + SQUARE(a.y - b.y))

SpriteManager::SpriteManager(Map* _nowMap, UIManager* _uiManager, ItemManager* _itemManager) : 
	nowMap{ _nowMap }, uiManager{ _uiManager }, itemManager{ _itemManager }, id {1}
{
	allSprites = new std::vector<std::shared_ptr<Sprite>>();
	init();

	auto& event = EventSystem::getInstance();
	event.subscribe<std::pair<int, sf::Vector2i>>("SPAWN_ENEMY", [=](const std::pair<int, sf::Vector2i> pair) { spawnEnemy(pair); });
}

void SpriteManager::init()
{
	id = 1;
	int i = 0;
	while (i < allSprites->size())
	{
		if ((*allSprites)[i]->id != 0)
		{
			(*allSprites)[i].reset();
			allSprites->erase(allSprites->begin() + i);
		}
		else
		{
			i++;
		}
	}

	enemys.clear();

	auto& data = Data::getInstance();
	PlayerDef plDef = data.getPlayerData();

	if (!player)
	{
		bool flag = false;
		for (auto sp : nowMap->getMapSprites())
		{
			if (sp.spriteDefId == 0)
			{
				auto def = spriteDefs[sp.spriteDefId];
				createPlayer(sp, def, plDef);
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			createDefaultPlayer(plDef);
		}
	}


	for (auto sp : nowMap->getMapSprites()) {
		createSpriteFromMapSprite(sp);
	}
}

void SpriteManager::createSpriteFromMapSprite(MapSprite mapSprite)
{
	auto def = spriteDefs[mapSprite.spriteDefId];

	if (def.type == SpriteType::Enemy)
	{
		createEnemy(mapSprite, def);
	}
	else if (def.type == SpriteType::NPC)
	{
		createNpc(mapSprite, def);
	}
	else if (def.type == SpriteType::Convertor)
	{
		createConverter(mapSprite, def);
	}
	
	id++;
}

void SpriteManager::createConverter(MapSprite mapSprite, SpriteDef def)
{
	auto cDef = converterDefs[def.texture - ENEMY_MAX_INDEX];
	auto converter = std::make_shared<Converter>(Converter(def, mapSprite, enemyDefs[mapSprite.spriteDefId], cDef, id));

	if (converter->spMap.nowHealPoint <= 0.0f) { return; }
	
	converter->changeState(Attack);
	enemys.push_back(converter.get());
	nowMap->setupBlockmap(converter.get());
	allSprites->push_back(std::move(converter));

	id++;
}

void SpriteManager::createEnemy(MapSprite mapSprite, SpriteDef def)
{
	if (mapSprite.spriteDefId == 0) return;

	auto enemy = std::make_shared<Enemy>(def, mapSprite, enemyDefs[mapSprite.spriteDefId], id);
	
	if (enemy->spMap.nowHealPoint <= 0.0f)
	{
		enemy->changeState(Dead);
	}
	else
	{
		enemy->changeState(Attack);
		enemys.push_back(enemy.get());
		nowMap->setupBlockmap(enemy.get());
	}

	allSprites->push_back(std::move(enemy));
}

void SpriteManager::createNpc(MapSprite mapSprite, SpriteDef def)
{
	auto npcDef = npcDefs[def.texture - ENEMY_MAX_INDEX - 4];

	if (npcDef.type == TraderNpcType)
	{
		TraderDef tradeDef;
		for (auto t : traderDefs)
		{
			if (t.startKey == npcDef.startKey)
			{
				tradeDef = t;
				break;
			}
		}

		allSprites->push_back(std::make_shared<TradeNpc>(TradeNpc(def, mapSprite, tradeDef, itemManager, 
			uiManager, player.get(), id)));
	}
	else if (npcDef.type == Traveler)
	{
		allSprites->push_back(std::make_shared<TravelerNpc>(TravelerNpc(def, mapSprite, npcDef, uiManager, itemManager, player.get(), id)));
	}
	else if (npcDef.type == ChangerNpcType)
	{
		allSprites->push_back(std::make_shared<ChangerNpc>(ChangerNpc(def, mapSprite, npcDef, uiManager, itemManager, player.get(), id)));
	}
	else if (npcDef.type == PortalNpcType)
	{
		allSprites->push_back(std::make_shared<PortalNpc>(PortalNpc(def, mapSprite, npcDef, uiManager, itemManager, player.get(), id)));
	}
	else if (npcDef.type == MechanicNpcType)
	{
		allSprites->push_back(std::make_shared<MechanicNpc>(MechanicNpc(def, mapSprite, npcDef, uiManager, itemManager, player.get(), id)));
	}
	else if (npcDef.type == QuestNpcType)
	{
		allSprites->push_back(std::make_shared<QuestNpc>(QuestNpc(def, mapSprite, npcDef, uiManager, itemManager, player.get(), id)));
	}
	else
	{
		allSprites->push_back(std::make_shared<Npc>(Npc(def, mapSprite, uiManager, player.get(), npcDef, id)));
	}

	nowMap->setupBlockmap(allSprites->back().get());
}

void SpriteManager::createPlayer(MapSprite mapSprite, SpriteDef def, PlayerDef plDef)
{
	auto enemy = std::make_shared<Enemy>(def, mapSprite, enemyDefs[mapSprite.spriteDefId], id);

	enemy->id = 0;
	enemy->enemyDef.maxHealpoint = plDef.maxHp;
	enemy->spMap.nowHealPoint = plDef.nowHp;
	player = std::make_unique<Player>(enemy.get(), plDef, nowMap);
	player->patrons = plDef.countpantrons;
	nowMap->setupBlockmap(enemy.get());

	allSprites->push_back(std::move(enemy));
}

void SpriteManager::createDefaultPlayer(PlayerDef plDef)
{
	auto def = spriteDefs[0];
	auto enDef = enemyDefs[0];
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

void SpriteManager::resetMap(Map* newMap, sf::Vector2f pos)
{
	nowMap = newMap;
	init();

	if (pos.x != 0.0f && pos.y != 0) // Убрать проверку потом 
	{
		player->enemy->spMap.position = pos;
	}
	player->setNemMap(newMap);
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

		if (enemy->spMap.nowHealPoint <= 0.0f)
		{
			killEnemy(enemy);
		}
	}

	aiControler(deltaTime);
}

void SpriteManager::aiControler(float deltaTime)
{
	for (size_t i = 0; i < enemys.size(); i++)
	{
		if (enemys[i]->isAtack && enemys[i]->nowTimeAtack >= enemys[i]->enemyDef.timeBettwenAtack)
		{
			enemys[i]->isAtack = false;

			if (isEnemyHit(enemys[i])) enemys[i]->attack(player.get());
			if (enemys[i]->spDef.type == SpriteType::Convertor || 
				enemys[i]->spDef.type == SpriteType::Boss) { break; }
		}

		float distance = GETDIST(enemys[i]->spMap.position, player->enemy->spMap.position);
		auto newState = determineNewState(enemys[i], distance);

		sf::Vector2f toPlayerDir = player->enemy->spMap.position - enemys[i]->spMap.position;

		float angle = enemys[i]->spMap.angle * PI / 180.0f;
		sf::Vector2f dir{ cos(angle), sin(angle) };

		if (newState == Run && !enemys[i]->isAtack)
		{
			if (Random::bitRandom() > 0.7f) enemys[i]->spMap.angle = std::atan2(toPlayerDir.y, toPlayerDir.x) * 180.0f / PI;
			enemys[i]->move(nowMap, enemys[i]->enemyDef.speed * deltaTime * dir);

			enemys[i]->changeState(Run);
		}
		else if (newState == Attack)
		{
			if (!enemys[i]->canChangeState()) continue;

			if (enemys[i]->isCanAttack)
			{
				enemys[i]->changeState(Attack);
			}
			else
			{
				if (Random::bitRandom() > 0.3f) enemys[i]->spMap.angle = std::atan2(toPlayerDir.y, toPlayerDir.x) * 180.0f / PI;
			}
		}
		else if (newState == Stay)
		{
			enemys[i]->changeState(Stay);
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
	else
	{
		return Stay;
	}
}

bool SpriteManager::isEnemyHit(Enemy* enemy)
{
	if (enemy->spDef.type == SpriteType::Convertor || enemy->spDef.type == SpriteType::Boss) return true;

	float angle = enemy->spMap.angle * PI / 180.0f;
	sf::Vector2f dir{ cos(angle), sin(angle) };

	RayHit hit = raycast(nowMap, enemy->spMap.position, dir, true, enemy, enemy->enemyDef.atackDist);
	if (hit.sprite)
	{
		if (hit.sprite == dynamic_cast<Sprite*>(player->enemy))
		{
			return true && Random::bitRandom() < 0.6f;
		}
	}
	return false;
}

void SpriteManager::spawnEnemy(std::pair<int, sf::Vector2i> pair)
{
	int x0 = pair.second.x - SPAWN_RADIUS, x1 = pair.second.x + SPAWN_RADIUS;
	int y0 = pair.second.y - SPAWN_RADIUS, y1 = pair.second.y + SPAWN_RADIUS;

	std::vector<sf::Vector2i> posVec;

	for (int x = x0; x < x1; x++)
	{
		for (int y = y0; y < y1; y++)
		{
			if (nowMap->blockMap[y][x].size() == 0) { posVec.push_back({ x,y }); }
		}
	}

	auto index = Random::intRandom(0, posVec.size() - 1);
	auto enemyDef = enemyDefs[pair.first];
	auto spDef = spriteDefs[pair.first];
	MapSprite spMap = {spDef.texture + 1, (sf::Vector2f)posVec[index], -90.0f, enemyDef.maxHealpoint};

	createEnemy(spMap, spDef);
}

void SpriteManager::killEnemy(Enemy* enem)
{
	enem->changeState(Dead);

	int details = Random::intRandom((int)(enem->enemyDef.midleDrop * 0.8f), (int)(enem->enemyDef.midleDrop * 1.2f));
	player->details += details;
	auto& questM = QuestManager::getInstance();
	questM.updateQuests(CollectionDetails, details);
	questM.updateQuests(KillMonster, 1);

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