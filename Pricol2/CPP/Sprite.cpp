#include "Sprite.h"
#include "UIManeger.h"
#include "ItemManager.h"
#include "Map.h"
#include "Weapon.h"
#include "Player.h"

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

Npc::Npc(SpriteDef _spDef, MapSprite _spMap, UIManager* _uiManager, Player* _player, NpcDef npcDef, int _id) :
	Sprite(_spDef, _spMap, _id), npcDefData{ npcDef }, player{ _player }, uiManager {_uiManager}, nowKey{ npcDef.startKey }
{ textSize = texture->getSize().y; }

void Npc::setEndFunc(std::function<void()> _endFunc) { endFunc = _endFunc; }

void Npc::init()
{
	uiManager->deleteNow();

	auto& data = Data::getInstance();
	auto keys = data.getKeys(nowKey);

	std::map<int, std::wstring, std::greater<int>> variants;

	for (int i = 0; i < keys.size(); i++)
	{
		auto d = data.getText(keys[i]);
		variants[d.second] = d.first;
	}

	uiManager->initDialog(variants, spDef.name);
}

void Npc::stop()
{
	uiManager->deleteNow();
	nowKey = npcDefData.startKey;
	endFunc();
	endFunc = nullptr;
}

void Npc::use() {}

void Npc::update(int chooseKey)
{
	nowKey = chooseKey;
	check();
}

void Npc::check()
{
	if (nowKey == 0)
	{
		stop();
	}
	else
	{
		init();
	}
}

FuncNpc::FuncNpc(SpriteDef spDef, MapSprite spMap, NpcDef npcDef, ItemManager* _itemManager,
	UIManager* uiManager, Player* _player, int _id) :
	Npc(spDef, spMap, uiManager, _player, npcDef, _id), isFunc{false}, choose{-1}, itemManager{_itemManager}{}

void FuncNpc::stop()
{
	Npc::stop();

	isFunc = false;
	choose = -1;
}

void FuncNpc::update(int chooseKey)
{
	nowKey = chooseKey;

	if (nowKey == 999)
	{
		isFunc = true;
		init();
		return;
	}

	if (!isFunc)
	{
		Npc::check();
	}
	else
	{
		check();
	}
}

void FuncNpc::check()
{
	if (nowKey == -100)
	{
		stop();
	}
	else if (nowKey == -200)
	{
		use();
	}
	else
	{
		choose = nowKey;
	}
}

TradeNpc::TradeNpc(SpriteDef spDef, MapSprite spMap, TraderDef _tradeDef,
	ItemManager* _itemManager, UIManager* uiManager, Player* _player, int _id) : 
	FuncNpc(spDef, spMap, NpcDef{Trader, _tradeDef.startKey}, _itemManager, uiManager, _player, _id), tradeDef{_tradeDef} {}

void TradeNpc::init()
{
	if (!isFunc)
	{
		Npc::init();
	}
	else
	{
		uiManager->deleteNow();

		auto result = std::map<int, Itemble*>();
		for (int i = 0; i < tradeDef.title.size(); i++)
		{
			result[tradeDef.title[i]] = itemManager->getItemble(tradeDef.title[i]);
		}

		uiManager->initTrade(result, player);
	}
}

void TradeNpc::use()
{
	if (choose == -1) return;

	Itemble* res = itemManager->getItemble(choose);

	if (res->cost > player->money) return;

	player->money -= res->cost;
	player->takeItem(res);

	choose = -1;

	init();
}

TravelerNpc::TravelerNpc(SpriteDef spDef, MapSprite spMap, NpcDef npcDef, 
	UIManager* uiManager, ItemManager* _itemManager, Player* _player, int _id) :
	FuncNpc(spDef, spMap, npcDef, _itemManager, uiManager, _player, _id) {}

void TravelerNpc::init()
{
	if (!isFunc)
	{
		Npc::init();
	}
	else
	{
		uiManager->deleteNow();

		auto result = std::map<int, Itemble*>();
		for (int i = 0; i < travelerDefs.size(); i++)
		{
			result[i] = itemManager->getItemble(travelerDefs[i].id);
		}

		uiManager->initTrade(result, player);
	}
}

void TravelerNpc::use()
{
	if (choose == -1) return;

	if (travelerDefs[choose].cost > player->money) return;

	player->money -= travelerDefs[choose].cost;

	int temp = travelerDefs[choose].effect;
	stop();

	auto& event = EventSystem::getInstance();
	event.trigger<int>("SWAPLOC", temp);
}

ChangerNpc::ChangerNpc(SpriteDef spDef, MapSprite spMap, NpcDef npcDef, 
	UIManager* uiManager, ItemManager* itemManager, Player* _player, int _id) : 
	FuncNpc(spDef, spMap, npcDef, itemManager, uiManager, _player, _id)
{
	auto& state = GameState::getInstance();
	coef = state.data.changerCoef;
}

void ChangerNpc::init()
{
	if (!isFunc)
	{
		Npc::init();
	}
	else
	{
		uiManager->deleteNow();

		uiManager->initChanger(coef, player);
	}
}

void ChangerNpc::use()
{
	if (player->details - 10 < 0) return;

	player->details -= 10;
	player->money += 10 * coef;

	init();
}

PortalNpc::PortalNpc(SpriteDef spDef, MapSprite spMap, NpcDef npcDef, 
	UIManager* uiManager, ItemManager* itemManager, Player* player, int _id) :
	FuncNpc(spDef, spMap, npcDef, itemManager, uiManager, player, _id) {}

void PortalNpc::init()
{
	if (!isFunc)
	{
		Npc::init();
	}
	else
	{
		FuncNpc::stop();

		auto& event = EventSystem::getInstance();
		event.trigger<int>("SWAPLOC", BASE_N);
	}
}

void PortalNpc::use() {}

MechanicNpc::MechanicNpc(SpriteDef spDef, MapSprite spMap, NpcDef npcDef,
	UIManager* uiManager, ItemManager* itemManager, Player* player, int _id) : 
	FuncNpc(spDef, spMap, npcDef, itemManager, uiManager, player, _id), nowGun{ nullptr } {}

void MechanicNpc::init()
{
	if (!isFunc)
	{
		Npc::init();
	}
	else
	{
		uiManager->deleteNow();

		uiManager->initMechanic(player, nowGun);
	}
}

void MechanicNpc::use()
{
	if (choose == -1) return;

	if (!nowGun) return;



}

void MechanicNpc::check()
{
	if (nowKey == -100)
	{
		stop();
	}
	else if (nowKey == -200)
	{
		use();
	}
	else
	{
		if (nowKey == 1 || nowKey == 2)
		{
			nowGun = player->guns[nowKey];
			return;
		}

		choose = nowKey;
	}
}
