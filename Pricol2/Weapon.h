#pragma once
#ifndef WEAPON
#define WEAPON

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <array>
#include <functional>

#include "Sprite.h"
#include "Animation.h"

class Weapon
{
public:
	Weapon(float _timeBetewen, Animator<sf::Texture*> anim);
	Weapon() = default;

	virtual void update(float dt)
	{
		if (nowTime >= timeBetwen) return;
		nowTime += dt;
		weaponAnimator.update(dt);
	}

	virtual void drawWeapon(sf::RenderWindow& window)
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

	virtual bool isCanUsed()
	{
		if (nowTime >= timeBetwen) return true;
		return false;
	}

	virtual void startAnimation(int number)
	{
		nowTime = 0.0f;
		weaponAnimator.setAnimation(number);
	}

	virtual void ussing(Sprite* sp) = 0;
private:
	Animator<sf::Texture*> weaponAnimator;
	float timeBetwen, nowTime;
};

class Gun : public Weapon
{
public:
	Gun(float _damage, int maxCnt, float _timeBetewen, float _timeBetewenReset, Animator<sf::Texture*>& anim,
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
	Gun() = default;

	void setSound(sf::SoundBuffer& shut, sf::SoundBuffer& reset, sf::SoundBuffer& cantShut) 
	{ 
		shutSound.setBuffer(shut);
		resetSound.setBuffer(reset);
		cantShutSound.setBuffer(cantShut);
	}

	void update(float dt) override
	{
		Weapon::update(dt);
		if (nowTimeBetwenReset >= timeBetwenReset) return;
		nowTimeBetwenReset += dt;
	}

	void resetPatron()
	{
		if (nowTimeBetwenReset >= timeBetwenReset && nowCount < maxCountPotron && isCanUsed())
		{
			resetFn(this);
			nowTimeBetwenReset = 0;
			resetSound.play();
		}
	}

	void ussing(Sprite* sp) override
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

	int nowCount;
private:
	float timeBetwenReset, nowTimeBetwenReset;
	std::function<void(Sprite* sp)> shutFn;
	std::function<void(Gun* gun)> resetFn;
	float damage;
	int maxCountPotron;
	sf::Sound shutSound;
	sf::Sound resetSound;
	sf::Sound cantShutSound;
};

#endif // !WEAPON

