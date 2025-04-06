#include "Sprite.h"
#include "DialogSystem.h"
#include "Map.h"

Sprite::Sprite(SpriteDef _spDef, MapSprite _spMap, int _id) :
	spDef{ _spDef }, spMap{ _spMap }, id{ _id }, isDamages{ false }
{
	timeAtecked = 0;

	if (spDef.texture != -1)
	{
		texture = &Resources::spritesTextures[spDef.texture];
		textSize = texture->getSize().x / 8;
	}

	float frameTime = 1.0f / spDef.speed;

	auto stay = Animation<int>({ {0,0} });

	auto run = Animation<int>({
		{0, 1},
		{ frameTime * 1, 2 },
		{ frameTime * 2, 3 },
		{ frameTime * 3, 4 },
		{ frameTime * 4, 4 } });

	auto atack = Animation<int>({
		{ frameTime * 1, 5 },
		{ frameTime * 2, 6 },
		{ frameTime * 3, 7 },
		{ frameTime * 4, 8 } });

	auto dead = Animation<int>({ {0,9} });

	animr = Animator<int>(0, {stay, run, atack, dead});
}

void Sprite::move(Map* map, sf::Vector2f move)
{
	if (move == sf::Vector2f()) return;

	float xOffset = move.x > 0 ? spDef.size / 2.0f : -spDef.size / 2.0f;
	float yOffset = move.y > 0 ? spDef.size / 2.0f : -spDef.size / 2.0f;
	if (!checkCollision(map, { spMap.position.x + move.x + xOffset, spMap.position.y }, true))
	{
		spMap.position.x += move.x;
	}
	if (!checkCollision(map, { spMap.position.x, spMap.position.y + move.y + yOffset }, false))
	{
		spMap.position.y += move.y;
	}

	map->setupBlockmap(this);
}

void Sprite::update(float deltaTime)
{
	if (state == Dead) return;

	animr.update(deltaTime);

	if (timeAtecked >= 0.5f)
	{
		timeAtecked = 0;
		isDamages = false;
		return;
	}

	timeAtecked += deltaTime;
}

int Sprite::getTextIndex()
{
	return animr.get();
}

void Sprite::changeState(SpriteState _state)
{
	if (_state == Run)
	{
		if (state != Run)
		{
			animr.setAnimation(1, true);
		}
	}
	else if (_state == Stay)
	{
		animr.setAnimation(0);
	}
	else if (_state == Dead)
	{
		animr.setAnimation(3, true);
	}

	state = _state;
}

void Sprite::takeDamage(float damage)
{
	if (state == Dead) return;

	spMap.nowHealPoint -= damage;
	SoundManager::playSound(Resources::takeDamage, 40);

	if (spMap.nowHealPoint <= 0.0f)
	{
		isDamages = false;
		state = Killes;
		return;
	}

	isDamages = true;
	timeAtecked = 0;
}

bool Sprite::checkCollision(Map* map, sf::Vector2f newPos, bool xAxis)
{
	sf::Vector2f thisSize{ spDef.size / 2.0f, spDef.size / 2.0f };
	sf::Vector2f thisStart = newPos - thisSize;
	sf::Vector2f thisEnd = newPos + thisSize;

	if (xAxis) {
		if (map->GetOnGrid(newPos.x, newPos.y, WALL_LAYER)) { return true; }

		auto curSp = map->getBlockMap((sf::Vector2i)newPos);
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
		if (map->GetOnGrid(newPos.x, newPos.y, WALL_LAYER)) { return true; }

		const auto& set = map->getBlockMap({ (int)newPos.x, (int)newPos.y });
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
	Sprite(_spDef, _spMap, _id), npcDefData{ npcDef[npcDefId]}, dialog{_dialog} {}


void Npc::use()
{
	dialog->start(npcDefData.startKey, spDef.name);
}