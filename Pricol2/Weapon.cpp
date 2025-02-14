#include "Weapon.h"

Weapon::Weapon(float _timeBetewen, Animator<sf::Texture*> anim)
	: timeBetwen{ _timeBetewen }, weaponAnimator{ anim } {
	nowTime = _timeBetewen;
}

void Weapon::update(float dt)
{
	if (nowTime >= timeBetwen) return;
	nowTime += dt;
	weaponAnimator.update(dt);
}

void Weapon::drawWeapon(sf::RenderWindow& window)
{
	sf::Texture* tex = weaponAnimator.get();
	if (tex)
	{
		sf::Sprite weapon{ *tex };
		weapon.setOrigin(tex->getSize().x / 2.0f, tex->getSize().y);
		weapon.setPosition(window.getSize().x / 2.0f, window.getSize().y);
		weapon.scale(2.5f, 2.5f);
		window.draw(weapon);
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

Gun::Gun(float _damage, int maxCnt, float _timeBetewen, float _timeBetewenReset, Animator<sf::Texture*>& anim,
	std::function<void(Sprite* sp)> _fn, std::function<void(Gun* gun)> _resetFn) : Weapon(_timeBetewen, anim)
{
	nowCount = maxCnt;
	nowTimeBetwenReset = _timeBetewenReset;
	timeBetwenReset = _timeBetewenReset;
	maxCountPotron = maxCnt;
	damage = _damage;
	shutFn = _fn;
	resetFn = _resetFn;
}

void Gun::setSound(sf::SoundBuffer& shut, sf::SoundBuffer& reset, sf::SoundBuffer& cantShut)
{
	shutSound.setBuffer(shut);
	resetSound.setBuffer(reset);
	cantShutSound.setBuffer(cantShut);
}

void Gun::update(float dt)
{
	Weapon::update(dt);
	if (nowTimeBetwenReset >= timeBetwenReset) return;
	nowTimeBetwenReset += dt;
}

void Gun::resetPatron()
{
	if (nowTimeBetwenReset >= timeBetwenReset && nowCount < maxCountPotron && isCanUsed())
	{
		resetFn(this);
		nowTimeBetwenReset = 0;
		resetSound.play();
	}
}

void Gun::ussing(Sprite* sp) 
{
	if (nowCount == 0)
	{
		cantShutSound.play();
		return;
	}
	if (isCanUsed() && nowCount > 0 && nowTimeBetwenReset >= timeBetwenReset)
	{
		if (sp != nullptr) shutFn(sp);
		nowCount--;
		shutSound.play();
		startAnimation(0);
	}
}