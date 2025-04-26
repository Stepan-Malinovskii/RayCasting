#include "Sprite.h"
#include "DialogSystem.h"
#include "Map.h"

Sprite::Sprite(SpriteDef _spDef, MapSprite _spMap, int _id) :
	spDef{ _spDef }, spMap{ _spMap }, id{ _id }
{
	if (spDef.texture != -1)
	{
		texture = &Resources::spritesTextures[spDef.texture];
		textSize = texture->getSize().x / 8;
	}
}

std::pair<int, bool> Sprite::getTextIndex() { return { 0, false }; }

Enemy::Enemy(SpriteDef spDef, MapSprite spMap, EnemyDef _enemyDef, int id) :
	Sprite(spDef, spMap, id), enemyDef{ _enemyDef }, state{ Stay }, timeAtecked{ 0.5f }, isDamaged{ false }, nowTimeAtack{ 0 }
{
	float frameTime = 1.0f / enemyDef.speed;
	auto stay = Animation<int>({ {0,0} });

	int index = 1;
	Animation<int> run;
	if (enemyDef.isCanRun)
	{
		run = Animation<int>({
		{ frameTime * 0, index++ },
		{ frameTime * 1, index++ },
		{ frameTime * 2, index++ },
		{ frameTime * 3, index },
		{ frameTime * 4, index++ } });
	}
	else
	{
		run = stay;
	}

	frameTime = 1.0f / 3;

	auto atack = Animation<int>({
	{ frameTime * 0, index++ },
	{ frameTime * 1, index++ },
	{ frameTime * 2, index },
	{ frameTime * 3, index++ } });

	auto dead = Animation<int>({ {0, index} });

	animr = Animator<int>(0, { stay, run, atack, dead });
}

std::pair<int, bool> Enemy::getTextIndex() { return { animr.get(), isDamaged }; }

void Enemy::move(Map* map, sf::Vector2f move)
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

void Enemy::update(float deltaTime)
{
	if (state == Dead) return;

	animr.update(deltaTime);

	updateTimeSinceLastAttack(deltaTime);
	updateTimeSinceDamaged(deltaTime);
}

void Enemy::updateTimeSinceLastAttack(float deltaTime)
{
	if (nowTimeAtack >= enemyDef.timeBettwenAtack)
	{
		isCanAttack = true;
	}
	else
	{
		nowTimeAtack += deltaTime;
	}
}

void Enemy::updateTimeSinceDamaged(float deltaTime)
{
	if (timeAtecked < 0.5f)
	{
		timeAtecked += deltaTime;
	}
	else
	{
		isDamaged = false;
	}
}

bool Enemy::canChangeState()
{
	return animr.get() == 0 || animr.isLopping == true;
}

void Enemy::changeState(EnemyState newState)
{
	if (newState == Stay)
	{
		animr.setAnimation(0);
	}
	else if (newState == Run)
	{
		if (state != Run)
		{
			animr.setAnimation(1, true);
		}
	}
	else if (newState == Attack)
	{
		isCanAttack = false;
		animr.setAnimation(2);
		nowTimeAtack = 0.0f;
	}
	else if (newState == Dead)
	{
		animr.setAnimation(3);
		isDamaged = false;
	}

	state = newState;
}

void Enemy::takeDamage(float damage)
{
	if (state == Dead) return;

	spMap.nowHealPoint -= damage;
	SoundManager::playSound(Resources::takeDamage);

	isDamaged = true;
	timeAtecked = 0;
}

bool Enemy::checkCollision(Map* map, sf::Vector2f newPos, bool xAxis)
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
	Sprite(_spDef, _spMap, _id), npcDefData{ npcDef[npcDefId] }, dialog{ _dialog }
{
	textSize = texture->getSize().y;
}