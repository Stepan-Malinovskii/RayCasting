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

Gun::Gun(int _id, int _damage, int maxCnt, int nowCnt, float _timeBetewen, float maxDist, float _timeBetewenReset) 
	: Weapon(_timeBetewen, maxDist)
{
	id = _id;
	nowCount = nowCnt;
	nowTimeBetwenReset = _timeBetewenReset;
	timeBetwenReset = _timeBetewenReset;
	maxCountPotron = maxCnt;
	damage = _damage;
}

Gun::Gun(GunDef def) : Weapon(def.shutTime, def.maxDist)
{
	id = def.id;
	damage = def.damage;
	maxCountPotron = def.maxCount;
	nowCount = def.nowCount;
	nowTimeBetwenReset = def.resetTime;
	timeBetwenReset = def.resetTime;
}

void Gun::setResetFunc(std::function<void()> _resetFn) { resetFn = _resetFn; }

void Gun::setShutFunc(std::function<void(Sprite* sp, float dist)> _shutfn) { shutFn = _shutfn; }

void Gun::setSound(sf::SoundBuffer* shut, sf::SoundBuffer* reset, sf::SoundBuffer* cantShut)
{
	if (shut != nullptr) shutSound.setBuffer(*shut);
	if (reset != nullptr) resetSound.setBuffer(*reset);
	if (cantShut != nullptr) cantShutSound.setBuffer(*cantShut);
}

void Gun::update(float dt)
{
	Weapon::update(dt);
	if (resetFn != NULL)
	{
		if (nowTimeBetwenReset >= timeBetwenReset) return;
		nowTimeBetwenReset += dt;
	}
}

void Gun::resetPatron()
{
	if (resetFn != nullptr)
	{
		if (nowTimeBetwenReset >= timeBetwenReset && nowCount < maxCountPotron && isCanUsed())
		{
			resetFn();
			nowTimeBetwenReset = 0;
			resetSound.play();
			startAnimation(1);
		}
	}
}

void Gun::ussing(Sprite* sp, float dist) 
{
	if (nowCount == 0 && resetFn != nullptr)
	{
		cantShutSound.play();
		return;
	}
	else if (isCanUsed() && (nowTimeBetwenReset >= timeBetwenReset || resetFn == nullptr))
	{
		if (sp != nullptr) shutFn(sp, dist);
		nowCount--;
		shutSound.play();
		startAnimation(0);
	}
}