#include "Sprite.h"
#include "Map.h"

Sprite::Sprite(sf::Vector2f pos, float _size, int indText, int _id, float _angle, bool isDirect, float HP, SpriteType _type)
	: position{ pos }, size{ _size }, texture{ indText }, angle{ _angle },
	isDirectional{ isDirect }, healPoint{ HP }, type{ _type }, maxHealpoint{ HP }, id{ _id } {}

Sprite::Sprite(SpriteDef spDef, MapSprite spMap, int id) : Sprite(spMap.position, spDef.size, spDef.indexTexture, id, spMap.angle, spDef.isDirectional, spDef.maxHealpoint) {}

void Sprite::move(Map* map, sf::Vector2f move)
{
	if (move == sf::Vector2f()) return;

	float xOffset = move.x > 0 ? size / 2.0f : -size / 2.0f;
	float yOffset = move.y > 0 ? size / 2.0f : -size / 2.0f;
	if (!checkCollision(*map, { position.x + move.x + xOffset, position.y }, true))
	{
		position.x += move.x;
	}
	if (!checkCollision(*map, { position.x, position.y + move.y + yOffset }, false))
	{
		position.y += move.y;
	}

	map->setupBlockmap(this);
}

bool Sprite::checkCollision(const Map& map, sf::Vector2f newPos, bool xAxis)
{
	sf::Vector2f thisSize{ size / 2.0f, size / 2.0f };
	sf::Vector2f thisStart = newPos - thisSize;
	sf::Vector2f thisEnd = newPos + thisSize;

	if (xAxis) {
		if (map.GetOnGrid(newPos.x, newPos.y, WALL_LAYER)) { return true; }

		auto curSp = map.getBlockMap((sf::Vector2i)newPos);
		for (auto sp : curSp) {
			if (sp->size == 0.f || sp == this) continue;

			sf::Vector2f spriteSize = { sp->size / 2.f, sp->size / 2.f };
			sf::Vector2f spriteStart = sp->position - spriteSize;
			sf::Vector2f spriteEnd = sp->position + spriteSize;

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
			if (thing->size == 0.f || thing == this) { continue; }

			sf::Vector2f halfSize = { thing->size / 2.f, thing->size / 2.f };
			sf::Vector2f thingStart = thing->position - halfSize;
			sf::Vector2f thingEnd = thing->position + halfSize;

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