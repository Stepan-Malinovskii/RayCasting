#pragma once
#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include "Map.h"
#include "Resources.h"
#include "Animation.h"
#include "Weapon.h"
#include "Raycast.h"

constexpr float ROTATION_SPEED = 80.0f, VERTICAL_MOUSE_SPEED = 25.0f;

class Inventory;

struct PlayerDef
{
	float maxHp;
	float nowHp;
	float maxEnergy;
	float nowEnergy;
	float defence;
	float maxStrenght;
	float nowStrenght;
	int countpantrons;
	int money;
	int details;
	std::vector<int> gunsData;
};

class Player
{
public:
	Player(Enemy* sprite, PlayerDef def, Map* _nowMap);
	~Player();
	void setInventory(Inventory* invent);
	void updateMouseData(sf::Vector2f mousePos, float deltaTime);
	void checkBoost(bool isPressed, float deltaTime);
	void move(sf::Vector2f deltaPos, float deltaTime);
	void jump();
	Sprite* dialog();
	void takeDamage(float damage);
	void fire(int gun = -1);
	void swapGun(bool flag);
	void reloadingGun();
	Gun* setGun(Gun* gun, int pos);
	Gun* getGun(int pos);
	float getMoveSpeed();
	void takeItem(Itemble*, int cnt = 1);
	void heal();

	Gun* getNowGun();
	sf::Vector2f getDeltaShake();
	PlayerDef getPlayerDef();
	void setNemMap(Map* map);

	Enemy* enemy;
	Gun* kick;
	float pitch, posZ, maxEnergy, nowEnergy;
	float defence, nowStrenght, maxStrenght;
	float mouseSpeed;
	int patrons;
	int money;
	int details;
	Item* nowHeal;
	Gun* guns[3]{};
private:
	Inventory* invent;
	bool isJump, jumpFlag;
	Map* nowMap;
	sf::Vector2f shakeDelta;
	int nowGun;
	float nowSpeed, boostSpeed, shakeTime;

	void resetPlayer();
	void shakeCamera(float deltaTime, bool isRun);
	void gravity(float deltaTime);
};

#endif // !PLAYER
