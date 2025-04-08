#include "Weapon.h"
#include "Player.h"

Weapon::Weapon(float _timeBetewen, float _maxDist)
	: timeBetwen{ _timeBetewen }, maxDist{ _maxDist } {
	nowTime = _timeBetewen;
}

void Weapon::update(float dt)
{
	weaponAnimator.update(dt);
	if (nowTime >= timeBetwen) return;
	nowTime += dt;
}

void Weapon::setAnimator(Animator<sf::Texture*>&& anim) { weaponAnimator = anim; }

void Weapon::drawWeapon(sf::RenderTarget* window, sf::Vector2f delta)
{
	sf::Texture* tex = weaponAnimator.get();
	if (tex)
	{
		sf::Sprite weapon{ *tex };
		weapon.setOrigin(tex->getSize().x / 2.0f, tex->getSize().y);
		weapon.scale(2.5f, 2.5f);
		weapon.setPosition(SCREEN_W / 2.0f - 20 + delta.x, SCREEN_H + delta.y + 10);
		window->draw(weapon);
	}
}

bool Weapon::isCanUsed()
{
	if (nowTime >= timeBetwen) return true;
	return false;
}

void Weapon::startAnimation(int number)
{
	nowTime = 0.0f;
	weaponAnimator.setAnimation(number);
}

Itemble::Itemble(std::wstring _name, std::wstring _disc, int _cost, int _textureId) :
	name{ _name }, disc{ _disc }, cost{ _cost }, id{ _textureId } {}

Improve::Improve(ImproveDef def, int id) : Itemble(def.name, def.disc, def.cost, id),
type{ def.type }
{
	if (type == Damage)
	{
		setGetFunc([=](Gun* gun) {gun->damage = round(gun->damage * def.effect);});

		setDelFunc([=](Gun* gun) {gun->damage = round(gun->damage / def.effect);});
	}
	else if (type == Spread)
	{
		setGetFunc([=](Gun* gun) {gun->maxImpRad = round(gun->maxImpRad / def.effect);
		gun->nowRad = std::min(gun->maxImpRad, gun->nowRad);});

		setDelFunc([=](Gun* gun) {gun->maxImpRad = round(gun->maxImpRad * def.effect);
		gun->nowRad = std::min(gun->maxImpRad, gun->nowRad);});
	}
	else
	{
		setGetFunc([=](Gun* gun) {gun->maxCount = round(gun->maxCount * def.effect);
		gun->nowCount = std::min(gun->maxCount, gun->nowCount);});

		setDelFunc([=](Gun* gun) {gun->maxCount = round(gun->maxCount / def.effect);
		gun->nowCount = std::min(gun->maxCount, gun->nowCount);});
	}
}

void Improve::setGetFunc(std::function<void(Gun* gun)> _setEffect) { getImprove = _setEffect; }

void Improve::setDelFunc(std::function<void(Gun* gun)> _delEffect) { deleteImprove = _delEffect; }

Item::Item(ItemsDef def, int id) : Itemble(def.name, def.disc, def.cost, id),
type{ def.type }, maxUsing{ def.maxUSing }
{
	if (def.type == Heal)
	{
		setFunc([=](Player* pl) {pl->sprite->spMap.nowHealPoint += def.effect;
		pl->sprite->spMap.nowHealPoint = std::min(pl->sprite->enemyDef.maxHealpoint, pl->sprite->spMap.nowHealPoint);});
	}
	else if (def.type == MaxHeal)
	{
		setFunc([=](Player* pl) {
			if (Random::bitRandom() > 0.2f)
				pl->sprite->enemyDef.maxHealpoint += def.effect;
			else
				pl->sprite->enemyDef.maxHealpoint -= def.effect / 2;
			pl->sprite->spMap.nowHealPoint = std::min(pl->sprite->enemyDef.maxHealpoint, pl->sprite->spMap.nowHealPoint);});
	}
	else if (def.type == MaxEnergy)
	{
		setFunc([=](Player* pl) {
			if (Random::bitRandom() > 0.2f)
				pl->maxEnergy += def.effect;
			else
				pl->maxEnergy -= def.effect / 2;
			pl->nowEnergy = std::min(pl->maxEnergy, pl->nowEnergy);});
	}
	else if (def.type == Armor)
	{
		setFunc([=](Player* pl) {pl->defence = def.effect;
		pl->maxStrenght = def.maxUSing;
		pl->nowStrenght = def.maxUSing;});
	}
	else if (def.type == Patrons)
	{
		setFunc([=](Player* pl) {pl->patrons += def.effect;});
	}
}

void Item::setFunc(std::function<void(Player* sprite)> _useFunc) { useFunc = _useFunc; }

void Item::useItem(Player* sprite) { useFunc(sprite); }

Gun::Gun(GunDef def, bool _isReset, int id, int _gunId) : Weapon(def.shutTime, def.maxDist),
Itemble(def.name, def.disc, def.cost, id),
damage{ def.damage }, maxCount{ def.maxCount }, nowCount{ def.nowCount },
nowTimeBetwenReset{ def.resetTime }, timeBetwenReset{ def.resetTime }, isReset{ _isReset }, gunId{ _gunId }
{
	nowRad = 1;
	maxImpRad = 30;
	maxRad = 30;
}

void Gun::update(float dt)
{
	Weapon::update(dt);
	if (isReset)
	{
		if (nowTimeBetwenReset >= timeBetwenReset) return;
		nowTimeBetwenReset += dt;
	}
}

void Gun::updateRad(bool isRun, float deltaTime)
{
	if (isRun)
	{
		nowRad = std::min(nowRad + maxRad * deltaTime * 2, maxImpRad);
	}
	else
	{
		nowRad = std::max(nowRad - maxRad * deltaTime * 2, 1.0f);
	}
}

int Gun::resetPatron(int count)
{
	if (!isReset) return count;
	
	if (nowTimeBetwenReset < timeBetwenReset && nowCount >= maxCount && !isCanUsed()) return count;
	
	auto delta = maxCount - nowCount;

	if (delta < count)
	{
		count -= delta;
		nowCount = maxCount;
	}
	else if (count > 0)
	{
		nowCount += count;
		count = 0;
	}
	else
	{
		return count;
	}

	nowTimeBetwenReset = 0;
	SoundManager::playSound(Resources::gunsResetSound[gunId], 30);
	startAnimation(1);
	return count;
}

void Gun::ussing(Enemy* sp, float dist)
{
	if (nowCount == 0 && isReset)
	{
		SoundManager::playSound(Resources::gunCantShoutSound, 60);
		return;
	}
	else if (isCanUsed() && (nowTimeBetwenReset >= timeBetwenReset || !isReset))
	{
		if (sp)
		{
			if (Random::bitRandom() > (nowRad - 0.05f) / maxRad - 0.35f || nowRad == 1)
			{
				sp->takeDamage(damage * (dist < maxDist ? 1 : 0));
			}
		}

		nowCount--;
		SoundManager::playSound(Resources::gunsShutSound[gunId], 30);
		startAnimation(0);
	}
}

Improve* Gun::trySetImprove(Improve* improve)
{
	auto temp = improvement[improve->type];
	if (temp != nullptr) { temp->deleteImprove(this); }
	improvement[improve->type] = improve;
	improve->getImprove(this);
	return temp;
}

Improve* Gun::deleteImprove(ImproveType type)
{
	auto temp = improvement[type];
	if (temp != nullptr)
	{
		temp->deleteImprove(this);
		improvement.erase(type);
	}
	return temp;
}

GunData Gun::getGunData()
{
	std::vector<int> ids;
	for (auto im : improvement)
	{
		if (im.second != nullptr)
		{
			ids.push_back(im.second->id);
		}
	}

	return { id, nowCount, ids };
}