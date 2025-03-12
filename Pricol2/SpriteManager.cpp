#include "SpriteManager.h"

SpriteManager::SpriteManager(Map* _nowMap) : nowMap{ _nowMap }
{
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
		auto def = spriteDef[sp.idx];
		auto sprite = std::make_shared<Sprite>(def, sp, id);
		id++;

		if (sp.idx == 0) {
			player = std::make_unique<Player>(Player(sprite.get(), nowMap));
		}
		else
		{
			sprite->thinker = std::make_shared<ThinkerLogic>([](Sprite& sprite1, Map& map, float deltaT) {
				sprite1.angle += 100 * deltaT;
				});
		}

		sprites.push_back(sprite);
	}

	/*sprites[2]->thinker = std::make_shared<ThinkerLogic>([](Sprite& sprite, Map& map, float deltaT) {
		sprite.move(map, sf::Vector2f(0, 0.7f) * deltaT);
		});*/

	if (!player) {
		auto def = spriteDef[0];
		auto sprite = std::make_shared<Sprite>(sf::Vector2f{ 2,2 }, def.size, def.indexTexture, id);
		id++;
		sprites.push_back(sprite);
		player = std::make_unique<Player>(Player(sprite.get(), nowMap));
	}

	for (auto sp : sprites)
	{
		nowMap->setupBlockmap(sp.get());
	}
}

Player* SpriteManager::resetMap(Map* newMap)
{
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
			if (sp->healPoint <= 0.0f)
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
	nowMap->writeMapSprite(sprites);
}

SpriteManager::~SpriteManager() {}