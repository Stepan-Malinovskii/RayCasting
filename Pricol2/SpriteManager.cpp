#include "SpriteManager.h"

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
	for (auto sp : nowMap->getMapSprites())
	{
		auto def = spriteDef[sp.spriteDefId];
		std::shared_ptr<Sprite> sprite;


		if (sp.spriteDefId == 0)
		{
			PlayerDef plDef = data->getPlayerData();
			if (plDef.maxHp != -1)
			{
				def.maxHealpoint = plDef.maxHp;
				sp.nowHealPoint = plDef.nowHp;
			}
			sprite = std::make_shared<Sprite>(def, sp, id);
			player = std::make_unique<Player>(Player(sprite.get(), nowMap));

		}
		else
		{
			sprite = std::make_shared<Sprite>(def, sp, id);
		}

		sprites.push_back(sprite);
		id++;
	}

	if (!player) {
		auto def = spriteDef[0];
		auto sprite = std::make_shared<Sprite>(sf::Vector2f{ 2,2 }, def.size, def.texture, id, 100);
		id++;
		sprites.push_back(sprite);
		player = std::make_unique<Player>(Player(sprite.get(), nowMap));
	}

	for (auto sp : sprites)
	{
		nowMap->setupBlockmap(sp.get());
	}
}

Npc SpriteManager::getNpc(MapSprite* mSp)
{
	auto def = spriteDef[mSp->spriteDefId];
	Npc npc(def, *mSp, id, ENEMY_COUNT - mSp->spriteDefId, dialogSys);
	id++;
	return npc;
}

Player* SpriteManager::resetMap(Map* newMap)
{
	nowMap = newMap;
	init();
	return player.get();
}

Player* SpriteManager::getPlayer() { return player.get(); }

std::vector<std::shared_ptr<Sprite>>& SpriteManager::getSprites() { return sprites;  }

void SpriteManager::update(float deltaTime)
{
	for (auto sp : sprites)
	{
		if (sp->thinker != nullptr)
		{
			sp->thinker->update(*sp, *nowMap, deltaTime);
		}
	}

	for (auto sp : sprites)
	{
		if (sp.get() != nullptr)
		{
			if (sp->spMap.nowHealPoint <= 0.0f)
			{
				deleteSprite(sp);
			}
		}
	}
}

void SpriteManager::deleteSprite(std::shared_ptr<Sprite> sp)
{
	nowMap->deleteInBlockMap(sp.get());
	for (int i = 0; i < sprites.size();i++)
	{
		if (sprites[i]->id == sp->id)
		{
			sprites.erase(sprites.begin() + i);
			break;
		}
	}
}

void SpriteManager::saveSprite()
{
	data->savePlayerData(player.get());
	nowMap->writeMapSprite(sprites);
}

SpriteManager::~SpriteManager() {}