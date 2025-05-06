#include "Sprite.h"
#include "UIManeger.h"
#include "ItemManager.h"
#include "Map.h"
#include "Weapon.h"
#include "Player.h"

Sprite::Sprite(SpriteDef _spDef, MapSprite _spMap, int _id) :
	spDef{ _spDef }, spMap{ _spMap }, id{ _id }, textSize{0}, texture{nullptr}
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

void Enemy::death() 
{
	animr.setAnimation(3);
	isDamaged = false;
}

void Enemy::attack(Player* player)
{
	player->takeDamage(enemyDef.damage);
}

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
	if (state == Dead) return;

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
		isAtack = true;
		isCanAttack = false;
		animr.setAnimation(2);
		nowTimeAtack = 0.0f;
	}
	else if (newState == Dead)
	{
		death();
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

EnemyState Enemy::determineNewState(float dist)
{
	if (dist < enemyDef.attackDist)
	{
		return Attack;
	}
	else if (dist < TRIGER_DIST)
	{
		return Run;
	}
	else
	{
		return Stay;
	}
}

void Enemy::enemyMechenic(float dist, sf::Vector2f toPlayerDir, Map* nowMap, float deltaTime)
{
	auto newState = determineNewState(dist);

	float angle = spMap.angle * PI / 180.0f;
	sf::Vector2f dir{ cos(angle), sin(angle) };

	if (newState == Run && !isAtack)
	{
		if (Random::bitRandom() > 0.7f) spMap.angle = std::atan2(toPlayerDir.y, toPlayerDir.x) * 180.0f / PI;
		move(nowMap, enemyDef.speed * deltaTime * dir);

		changeState(Run);
	}
	else if (newState == Attack)
	{
		if (!canChangeState()) return;

		if (isCanAttack)
		{
			changeState(Attack);
		}
		else
		{
			if (Random::bitRandom() > 0.3f) spMap.angle = std::atan2(toPlayerDir.y, toPlayerDir.x) * 180.0f / PI;
		}
	}
	else if (newState == Stay)
	{
		changeState(Stay);
	}
}

bool Enemy::checkCollision(Map* map, sf::Vector2f newPos, bool xAxis)
{
	sf::Vector2f thisSize{ spDef.size / 2.0f, spDef.size / 2.0f };
	sf::Vector2f thisStart = newPos - thisSize;
	sf::Vector2f thisEnd = newPos + thisSize;

	if (xAxis) {
		if (map->GetOnGrid((int)newPos.x, (int)newPos.y, WALL_LAYER)) { return true; }

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
		if (map->GetOnGrid((int)newPos.x, (int)newPos.y, WALL_LAYER)) { return true; }

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
	FuncNpc(spDef, spMap, NpcDef{TraderNpcType, _tradeDef.startKey}, _itemManager, uiManager, _player, _id), tradeDef{_tradeDef} {}

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
		result[0] = itemManager->getItemble(travelerDefs[0].id);

		int maxSize = 1;
		auto& state = GameState::getInstance();
		int levelN = state.data.levelNumber;
		if (state.data.killFirst &&
			state.data.killSecond &&
			state.data.killTherd)
		{
			maxSize = 5;
		}
		else if (levelN > 12)
		{
			maxSize = 4;
		}
		else if (levelN > 9)
		{
			maxSize = 3;
		}
		else if (levelN > 6)
		{
			maxSize = 2;
		}

		for (int i = 1; i < maxSize; i++)
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
	auto& questM = QuestManager::getInstance();
	questM.updateQuests(CollectionMoney, 10 * coef);

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

		use();
	}
}

void PortalNpc::use()
{
	auto& event = EventSystem::getInstance();
	event.trigger<int>("SWAPLOC", BASE_N);
}

MechanicNpc::MechanicNpc(SpriteDef spDef, MapSprite spMap, NpcDef npcDef,
	UIManager* uiManager, ItemManager* itemManager, Player* player, int _id) : 
	FuncNpc(spDef, spMap, npcDef, itemManager, uiManager, player, _id), typeUpgade{ -1 } {}

void MechanicNpc::init()
{
	if (!isFunc)
	{
		Npc::init();
	}
	else
	{
		uiManager->deleteNow();

		uiManager->initMechanic(player, (choose == -1 ? nullptr : player->guns[choose]));
	}
}

void MechanicNpc::stop()
{
	FuncNpc::stop();
	typeUpgade = -1;
}

void MechanicNpc::use()
{
	if (choose == -1 || typeUpgade == -1) return;

	Gun* nowGun = player->guns[choose];

	if (player->money < 50 || player->details < 15 || nowGun->upgradeCount >= 5) return;

	player->money -= 50;
	player->details -= 15;
	
	Improve* imp = nullptr;
	if (typeUpgade == 101)
	{
		imp = nowGun->deleteImprove(Damage);
		nowGun->damage += 3;
	}
	else if (typeUpgade == 102)
	{
		imp = nowGun->deleteImprove(Magazin);
		nowGun->maxCount += 5;
	}
	else if (typeUpgade == 103)
	{
		imp = nowGun->deleteImprove(Spread);
		nowGun->maxRad += 3;
	}

	nowGun->trySetImprove(imp);
	nowGun->upgradeCount++;
	
	typeUpgade = -1;
	choose = -1;
	init();
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
	else if (nowKey == 1 || nowKey == 2)
	{
		choose = nowKey;
		init();
		return;
	}
	else
	{
		typeUpgade = nowKey;
	}
}

QuestNpc::QuestNpc(SpriteDef spDef, MapSprite spMap, NpcDef npcDef,
	UIManager* uiManager, ItemManager* itemManager, Player* player, int _id) :
	FuncNpc(spDef, spMap, npcDef, itemManager, uiManager, player, _id)  {}

void QuestNpc::init()
{
	if (!isFunc)
	{
		Npc::init();
	}
	else
	{
		uiManager->deleteNow();

		auto& questM = QuestManager::getInstance();
		uiManager->initQuest((choose == -1 ? nullptr : questM.quests[choose - 1]), player);
	}
}

void QuestNpc::use()
{
	if (choose == -1) return;

	auto& questM = QuestManager::getInstance();
	player->money += questM.deleteQuest(questM.quests[choose - 1]);

	choose = -1;
	init();
}

void QuestNpc::check()
{
	if (nowKey == -100)
	{
		stop();
	}
	else if (nowKey == -200)
	{
		use();
	}
	else if (nowKey == -300)
	{
		int type = Random::intRandom(0, 2);
		QuestData data;
		data.progress = 0;
		data.type = QuestType(type);

		if (type == 0)
		{
			data.target = Random::intRandom(5, 15);
			data.rewardCoins = Random::intRandom(data.target * 10, data.target * 15);
		}
		else if (type == 1)
		{
			data.target = Random::intRandom(100, 300);
			data.rewardCoins = Random::intRandom((int)(data.target * 1.1f), (int)(data.target * 1.3f));
		}
		else if (type == 2)
		{
			data.target = Random::intRandom(10, 25);
			data.rewardCoins = data.rewardCoins = Random::intRandom(data.target * 10, data.target * 15);
		}
		
		auto& questM = QuestManager::getInstance();
		questM.addQuest(data);

		init();
	}
	else if (nowKey == 1 || nowKey == 2 || nowKey == 3)
	{
		choose = nowKey;
		init();
	}
}

Converter::Converter(SpriteDef spDef, MapSprite spMap, EnemyDef enemyDef, ConverterDef cDef, int id) :
	Enemy(spDef, spMap, enemyDef, id), cDef{cDef}
{
	nowSpawnCount = (int)(cDef.maxSpawnCount * spMap.nowHealPoint / enemyDef.maxHealpoint);
	textSize = texture->getSize().y / 2;
	Animation<int> stay({ {0.0f, 0} });
	Animation<int> attack({ {0.0f, 0}, {enemyDef.timeBettwenAtack, 0 }, {enemyDef.timeBettwenAtack, 0 } });
	Animation<int> death({ {0.0f, 1} });
	animr = Animator<int>(0, { stay, {}, attack, death });
}

void Converter::death()
{
	auto& state = GameState::getInstance();
	if (spDef.texture == 13)
	{
		state.data.killFirst = true;
	}
	else if (spDef.texture == 14)
	{
		state.data.killSecond = true;
	}
	else if (spDef.texture == 15)
	{
		state.data.killTherd = true;
	}

	isDamaged = false;
	animr.setAnimation(3);
	auto& event = EventSystem::getInstance();
	event.trigger<sf::Vector2f>("SPAWN_PORTAL", spMap.position);
}

void Converter::attack(Player* plaer)
{
	nowSpawnCount--;
	auto& event = EventSystem::getInstance();
	event.trigger<std::pair<int, sf::Vector2i>>("SPAWN_ENEMY", { cDef.callingIndex[Random::intRandom(0, cDef.callingIndex.size() - 1)], (sf::Vector2i)spMap.position});
}

void Converter::changeState(EnemyState newState)
{
	if (state == Dead) return;

	if (newState == Attack)
	{
		isAtack = true;
		isCanAttack = false;
		animr.setAnimation(2);
		nowTimeAtack = 0.0f;
	}
	else if (newState == Dead)
	{
		death();
	}

	state = newState;
}

bool Converter::canChangeState()
{
	return !isAtack || nowSpawnCount > 0;
}
