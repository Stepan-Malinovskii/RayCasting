#include "Weapon.h"

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

void Weapon::setAnimator(Animator<sf::Texture*>& anim) { weaponAnimator = anim; }

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

Improve::Improve(ImproveType _type, std::wstring _name) : type{ _type }, name{ _name } {}

void Improve::setGetFunc(std::function<void(Gun* gun)> _setEffect) { getImprove = _setEffect; }

void Improve::setDelFunc(std::function<void(Gun* gun)> _delEffect) { deleteImprove = _delEffect; }

Gun::Gun(GunDef def, bool _isReset) : Weapon(def.shutTime, def.maxDist)
{
	nowRad = 1;
	maxRad = 30;
	id = def.id;
	damage = def.damage;
	maxCount = def.maxCount;
	nowCount = def.nowCount;
	nowTimeBetwenReset = def.resetTime;
	timeBetwenReset = def.resetTime;
	isReset = _isReset;
}

void Gun::setSound(sf::SoundBuffer* shut, sf::SoundBuffer* reset, sf::SoundBuffer* cantShut)
{
	if (shut != nullptr) shutSound.setBuffer(*shut);
	if (reset != nullptr) resetSound.setBuffer(*reset);
	if (cantShut != nullptr) cantShutSound.setBuffer(*cantShut);
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
		if (nowRad < maxRad)
			nowRad += maxRad * deltaTime * 2;
		else nowRad = maxRad;
	}
	else
	{
		if (nowRad > 1)
			nowRad -= maxRad * deltaTime * 2;
		else nowRad = 1;
	}
}

void Gun::resetPatron()
{
	if (isReset)
	{
		if (nowTimeBetwenReset >= timeBetwenReset && nowCount < maxCount && isCanUsed())
		{
			nowCount = maxCount;
			nowTimeBetwenReset = 0;
			resetSound.play();
			startAnimation(1);
		}
	}
}

void Gun::ussing(Sprite* sp, float dist) 
{
	if (nowCount == 0 && isReset)
	{
		cantShutSound.play();
		return;
	}
	else if (isCanUsed() && (nowTimeBetwenReset >= timeBetwenReset || !isReset))
	{
		if (sp != nullptr)
		{
			if (Random::bitRandom() > (nowRad - 0.1f) / maxRad - 0.1f || nowRad == 1)
			sp->takeDamage(damage * (dist < maxDist ? 1 : 0));
		}
		nowCount--;
		shutSound.play();
		startAnimation(0);
	}
}

bool Gun::trySetImprove(Improve* improve)
{
	if (improvement[improve->type] != nullptr) return false;
	improvement[improve->type] = improve;
	improve->getImprove(this);
	return true;
}

Improve* Gun::deleteImprove(ImproveType type)
{
	auto temp = improvement.extract(type);
	if (!temp.empty())
	{
		temp.mapped()->deleteImprove(this);
	}
	return temp.mapped();
}