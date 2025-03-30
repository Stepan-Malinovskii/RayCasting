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

class Gun;
class Player;

struct GunDef
{
	int damage;
	int maxCount;
	int nowCount;
	float shutTime;
	float maxDist;
	float resetTime;
	int cost;
	std::wstring name;
	std::wstring disc;
};

struct GunData
{
	int id;
	int nowCount;
	std::vector<int> improveId;
};

enum ImproveType
{
	Damage, Spread, Magazin
};

struct ImproveDef
{
	ImproveType type;
	std::wstring name;
	float effect;
	int cost;
	std::wstring disc;
};

enum ItemType
{
	MaxEnergy, MaxHeal, Heal, Armor
};

struct ItemsDef
{
	ItemType type;
	std::wstring name;
	int effect;
	int maxUSing;
	int cost;
	std::wstring disc;
};

class Itemble
{
public:
	Itemble(std::wstring name, std::wstring disc, int cost, int textureId);
	Itemble() = default;
	virtual ~Itemble() = default;
	std::wstring name;
	std::wstring disc;
	int cost;
	int id;
};

class Item : public Itemble
{
public:
	Item(ItemsDef def, int id);
	Item() = default;
	void setFunc(std::function<void(Player* player)> _useFunc);
	void useItem(Player* sprite);
	ItemType type;
	int maxUsing;
	std::function<void(Player* sprite)> useFunc;
};

class Improve : public Itemble
{
public:
	Improve(ImproveDef def, int id);
	Improve() = default;
	void setGetFunc(std::function<void(Gun* gun)> setEffect);
	void setDelFunc(std::function<void(Gun* gun)> delEffect);
	ImproveType type;
	std::function<void(Gun* gun)> getImprove;
	std::function<void(Gun* gun)> deleteImprove;
};

class Weapon
{
public:
	Weapon(float _timeBetewen, float maxDist);
	Weapon() = default;

	virtual void update(float dt);

	virtual void drawWeapon(sf::RenderTarget* window, sf::Vector2f delta);

	virtual bool isCanUsed();

	virtual void setAnimator(Animator<sf::Texture*>& anim);

	float maxDist;
protected:

	virtual void startAnimation(int number);

	virtual void ussing(Sprite* sp, float dist) = 0;

	Animator<sf::Texture*> weaponAnimator;
	float timeBetwen, nowTime;
};

class Gun : public Weapon, public Itemble
{
public:
	Gun(GunDef def, bool isReset, int id);
	Gun() = default;

	void setSound(sf::SoundBuffer* shut = nullptr,
		sf::SoundBuffer* reset = nullptr,
		sf::SoundBuffer* cantShut = nullptr);

	Improve* trySetImprove(Improve* improve);

	Improve* deleteImprove(ImproveType type);

	void update(float dt) override;

	void updateRad(bool isRun, float deltaTime);

	int resetPatron(int count);

	GunData getGunData();

	void ussing(Sprite* sp, float dist) override;

	bool isReset;
	int nowCount;
	int maxCount;
	int damage;
	float nowRad;
	float maxRad;
	std::map<ImproveType, Improve*> improvement;
private:
	float timeBetwenReset, nowTimeBetwenReset;
	sf::Sound shutSound;
	sf::Sound resetSound;
	sf::Sound cantShutSound;
};

#endif // !WEAPON