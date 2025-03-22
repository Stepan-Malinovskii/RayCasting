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
#include "Resources.h"
#include "Sprite.h"
#include "Animation.h"

class Weapon
{
public:
	Weapon(float _timeBetewen, float maxDist);
	Weapon() = default;

	virtual void update(float dt);

	virtual void drawWeapon(sf::RenderTarget* window, sf::Vector2f delta);

	virtual bool isCanUsed();

	virtual void setAnimator(Animator<sf::Texture*>& anim);

	virtual void startAnimation(int number);

	virtual void ussing(Sprite* sp, float dist) = 0;
	float maxDist;
private:
	Animator<sf::Texture*> weaponAnimator;
	float timeBetwen, nowTime;
};

class Gun : public Weapon
{
public:
	Gun(int _id, int _damage, int maxCnt, float _timeBetewen, float maxDist, float _timeBetewenReset);
	Gun() = default;

	void setSound(sf::SoundBuffer* shut = nullptr, sf::SoundBuffer* reset = nullptr, sf::SoundBuffer* cantShut = nullptr);

	void setResetFunc(std::function<void()> _resetFn);

	void setShutFunc(std::function<void(Sprite* sp, float dist)> _shutfn);

	void update(float dt) override;

	void resetPatron();

	void ussing(Sprite* sp, float dist) override;

	int nowCount;
	int maxCountPotron;
	int damage;
	int id;
	std::function<void()> resetFn;
private:
	float timeBetwenReset, nowTimeBetwenReset;
	std::function<void(Sprite* sp, float dist)> shutFn;
	sf::Sound shutSound;
	sf::Sound resetSound;
	sf::Sound cantShutSound;
};

#endif // !WEAPON

