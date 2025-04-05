#include "SpriteManager.h"

#define SQUARE(a) ((a) * (a))
#define GETDIST(a,b) (SQUARE(a.x - b.x) + SQUARE(a.y - b.y))

SpriteManager::SpriteManager(Map* _nowMap, Data* _data, Dialog* _dialogSys) : nowMap{ _nowMap }
{
	dialogSys = _dialogSys;
	data = _data;
	id = 0;
	init();
}

void SpriteManager::init()
{
	id = 0;
	sprites.clear();
	player = nullptr;
	PlayerDef plDef = data->getPlayerData();
	for (auto sp : nowMap->getMapSprites())
	{
		auto def = spriteDef[sp.spriteDefId];
		Sprite* sprite;


		if (sp.spriteDefId < ENEMY_COUNT)
		{
			enemys.push_back(std::make_shared<Sprite>(def, sp, id));
			sprite = enemys.back().get();

			if (sp.spriteDefId == 0)
			{
				def.maxHealpoint = plDef.maxHp;
				sp.nowHealPoint = plDef.nowHp;
				player = std::make_unique<Player>(Player(enemys.back().get(), plDef, nowMap));
			}
		}
		else
		{
			int in = sp.spriteDefId - ENEMY_COUNT;
			auto npc = std::make_shared<Npc>(Npc(def, sp, id, in, dialogSys));
			npcs.push_back(npc);
			sprite = npc.get();
		}

		sprites.push_back(sprite);
		id++;
	}

	if (!player) {
		auto def = spriteDef[0];
		def.maxHealpoint = plDef.maxHp;

		enemys.push_back(std::make_shared<Sprite>(def, MapSprite{ 0, {2.0f, 2.0f}, 90.0f, plDef.nowHp }, id));
		sprites.push_back(enemys.back().get());
		player = std::make_unique<Player>(Player(enemys.back().get(), plDef, nowMap));

		id++;
	}

	for (auto sp : sprites)
	{
		if (sp->spMap.nowHealPoint > 0.0f)
		{
			nowMap->setupBlockmap(sp);
			sp->changeState(Stay);
		}
		else
		{
			sp->changeState(Dead);
		}
	}
}

Player* SpriteManager::resetMap(Map* newMap)
{
	nowMap = newMap;
	init();
	return player.get();
}

Player* SpriteManager::getPlayer() { return player.get(); }

std::vector<Sprite*> SpriteManager::getDeteachSprite() { return sprites;  }

void SpriteManager::update(float deltaTime)
{
	for (auto sp : enemys)
	{
		if (sp->spDef.texture > ENEMY_COUNT - 1) continue;
		if (sp.get()->spDef.texture != -1)
		{
			float dist = GETDIST(sp->spMap.position, player->sprite->spMap.position);

			if (dist > 30)
			{
				if (sp->state < Killes)
				{
					sp->changeState(Stay);
				}
			}
			else if (dist < 20)
			{
				if (sp->state < Killes)
				{
					sp->changeState(Run);
				}
			}
		}
	}

	for (auto sp : enemys)
	{
		sp->update(deltaTime);

		if (sp->state == Killes)
		{
			sp->changeState(Dead);
			player->money += Random::intRandom((int)(sp->spDef.midleDrop * 0.8f), (int)(sp->spDef.midleDrop * 1.2f));
			deleteSprite(sp);
		}
	}
}

void SpriteManager::deleteSprite(std::shared_ptr<Sprite> sp)
{
	nowMap->deleteInBlockMap(sp.get());
}

SpriteManager::~SpriteManager() {}