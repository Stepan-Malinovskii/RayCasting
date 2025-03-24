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
#include "Randomizer.h"

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

struct GunDef
{
	int id;
	int damage;
	int maxCount;
	int nowCount;
	float shutTime;
	float maxDist;
	float resetTime;
};

class Gun : public Weapon
{
public:
	Gun(GunDef def, bool isReset);
	Gun() = default;

	void setSound(sf::SoundBuffer* shut = nullptr, 
				  sf::SoundBuffer* reset = nullptr, 
		          sf::SoundBuffer* cantShut = nullptr);

	void update(float dt) override;

	void updateRad(bool isRun, float deltaTime);

	void resetPatron();

	void ussing(Sprite* sp, float dist) override;

	bool isReset;
	int nowCount;
	int maxCount;
	float nowRad;
	float maxRad;
private:
	int damage;
	int id;
	float timeBetwenReset, nowTimeBetwenReset;
	sf::Sound shutSound;
	sf::Sound resetSound;
	sf::Sound cantShutSound;
};

#endif // !WEAPON

