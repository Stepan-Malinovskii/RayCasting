#include "Sprite.h"
#include "DialogSystem.h"
#include "Map.h"

Sprite::Sprite(sf::Vector2f pos, float _size, int indText, int _id, float HP, float _angle, bool isDirect, SpriteType _type)
	: spMap{indText + 1, pos, _angle, HP}, 
	spDef{ L"", _type, _size, HP, indText, isDirect }, id{ _id } 
{
	if (spDef.texture != -1)
	{
		texture = &Resources::spritesTextures[spDef.texture];
		textSize = texture->getSize().y;
	}
}

Sprite::Sprite(SpriteDef _spDef, MapSprite _spMap, int _id) : 
	spDef{ _spDef }, spMap{ _spMap }, id { _id } 
{
	if (spDef.texture != -1)
	{
		texture = &Resources::spritesTextures[spDef.texture];
		textSize = texture->getSize().y;
	}
}

void Sprite::move(Map* map, sf::Vector2f move)
{
	if (move == sf::Vector2f()) return;

	float xOffset = move.x > 0 ? spDef.size / 2.0f : -spDef.size / 2.0f;
	float yOffset = move.y > 0 ? spDef.size / 2.0f : -spDef.size / 2.0f;
	if (!checkCollision(*map, { spMap.position.x + move.x + xOffset, spMap.position.y }, true))
	{
		spMap.position.x += move.x;
	}
	if (!checkCollision(*map, { spMap.position.x, spMap.position.y + move.y + yOffset }, false))
	{
		spMap.position.y += move.y;
	}

	map->setupBlockmap(this);
}

bool Sprite::checkCollision(const Map& map, sf::Vector2f newPos, bool xAxis)
{
	sf::Vector2f thisSize{ spDef.size / 2.0f, spDef.size / 2.0f };
	sf::Vector2f thisStart = newPos - thisSize;
	sf::Vector2f thisEnd = newPos + thisSize;

	if (xAxis) {
		if (map.GetOnGrid(newPos.x, newPos.y, WALL_LAYER)) { return true; }

		auto curSp = map.getBlockMap((sf::Vector2i)newPos);
		for (auto sp : curSp) {
			if (sp->spDef.size == 0.f || sp == this) continue;

			sf::Vector2f spriteSize = { sp->spDef.size / 2.f, sp->spDef.size / 2.f };
			sf::Vector2f spriteStart = sp->spMap.position - spriteSize;
			sf::Vector2f spriteEnd = sp->spMap.position + spriteSize;

			float px = newPos.x;
			float py0 = thisStart.y, py1 = thisEnd.y, magnitude = py1 - py0;
			float t1 = (spriteStart.y - py0) / magnitude;
			float t2 = (spriteEnd.y - py0) / magnitude;

			float entry = std::fmin(t1, t2);
			float exit = std::fmax(t1, t2);

			bool yIntersect = exit > entry && exit > 0.f && entry < 1.f;
			if (px >= spriteStart.x && px <= spriteEnd.x && yIntersect) {
				return true;
			}
		}
	}
	else {
		if (map.GetOnGrid(newPos.x, newPos.y, WALL_LAYER)) { return true; }

		const auto& set = map.getBlockMap({ (int)newPos.x, (int)newPos.y });
		for (const auto& thing : set) {
			if (thing->spDef.size == 0.f || thing == this) { continue; }

			sf::Vector2f halfSize = { thing->spDef.size / 2.f, thing->spDef.size / 2.f };
			sf::Vector2f thingStart = thing->spMap.position - halfSize;
			sf::Vector2f thingEnd = thing->spMap.position + halfSize;

			float py = newPos.y;
			float px0 = thisStart.x, px1 = thisEnd.x, magnitude = px1 - px0;
			float t1 = (thingStart.x - px0) / magnitude;
			float t2 = (thingEnd.x - px0) / magnitude;

			float entry = std::fmin(t1, t2);
			float exit = std::fmax(t1, t2);

			bool xIntersect = exit > entry && exit > 0.f && entry < 1.f;
			if (py >= thingStart.y && py <= thingEnd.y && xIntersect) {
				return true;
			}
		}

	}

	return false;
}

Npc::Npc(SpriteDef _spDef, MapSprite _spMap, int _id, int npcDefId, Dialog* _dialog) :
	Sprite(_spDef, _spMap, _id), npcDefData{ npcDef[npcDefId]}, dialog{_dialog} { }


void Npc::use()
{
	dialog->start(this);
}