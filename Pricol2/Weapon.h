#pragma once
#ifndef WEAPON
#define WEAPON

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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
	Gun(float _damage, float _timeBetewen, Animator<sf::Texture*> anim,
		std::function<void(Sprite* sp)> _fn) : Weapon(_timeBetewen, anim)
	{
		damage = _damage;
		fn = _fn;
	}
	Gun() = default;

	void ussing(Sprite* sp) override
	{
		fn(sp);
	}
private:
	std::function<void(Sprite* sp)> fn;
	float damage;
};

#endif // !WEAPON

