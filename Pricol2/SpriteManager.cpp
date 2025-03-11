//#include "SpriteManager.h"
//
//SpriteManager::SpriteManager(Map* _nowMap) : nowMap{ _nowMap }
//{
//	init();
//}
//
//void SpriteManager::init()
//{
//	for (auto sp : nowMap->getMapSprites())
//	{
//		auto def = spriteDef[sp.idx];
//		auto sprite = std::make_shared<Sprite>(def, sp);
//		if (sp.idx == 0)
//		{
//			sprites.push_back(sprite);
//			player = std::make_unique<Player>(Player(sprite.get(), nowMap));
//		}
//		else
//		{
//			sprite->thinker = std::make_shared<ThinkerLogic>([](Sprite& sprite1, Map& map, float deltaT) {
//				sprite1.angle += 100 * deltaT; });
//		}
//
//		sprites.push_back(sprite);
//	}
//
//	if (!player) {
//		auto def = spriteDef[0];
//		auto sprite = std::make_shared<Sprite>(def, MapSprite{0, nowMap->plData.pos, nowMap->plData.angle});
//		sprites.push_back(sprite);
//		player = std::make_unique<Player>(Player(sprite.get(), nowMap));
//	}
//
//	for (auto sp : sprites)
//	{
//		nowMap->setInBlockMap(sp.get());
//	}
//}
//
//Player* SpriteManager::resetMap(Map* newMap)
//{
//	init();
//	return player.get();
//}
//
//Player* SpriteManager::getPlayer() { return player.get(); }
//
//std::vector<std::shared_ptr<Sprite>>& SpriteManager::getSprites() { return sprites;  }
//
//void SpriteManager::update(float deltaTime)
//{
//	for (auto sp : sprites)
//	{
//		if (sp->thinker != nullptr)
//		{
//			sp->thinker->update(*sp, *nowMap, deltaTime);
//		}
//	}
//
//	for (auto sp : sprites)
//	{
//		if (sp->healPoint <= 0.0f)
//		{
//			deleteSprite(sp);
//		}
//	}
//}
//
//void SpriteManager::deleteSprite(std::shared_ptr<Sprite> sp)
//{
//	nowMap->deleteInBlockMap(sp.get());
//	auto it = std::find(sprites.begin(), sprites.end(), sp);
//	if (it != sprites.end())
//	{
//		sprites.erase(it);
//	}
//	else
//	{
//		throw "ERROR SPRITE NOT DELETE";
//	}
//}
//
//SpriteManager::~SpriteManager() {}