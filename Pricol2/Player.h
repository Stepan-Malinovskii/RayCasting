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

constexpr float ROTATION_SPEED = 80.0f, MOUSE_TURN_SPEED = 0.08f, VERTICAL_MOUSE_SPEED = 25.0f;

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
	std::vector<std::pair<int, int>> itemData;
};


class Player
{
public:
	Player(Sprite* sprite, PlayerDef def, Map* _nowMap);

	void updateMouseData(sf::Vector2f mousePos, float deltaTime);
	void checkBoost(bool isPressed, float deltaTime);
	void move(sf::Vector2f deltaPos, float deltaTime);
	void jump();
	Sprite* dialog();
	void takeDamage(float damage);
	void fire(int gun = -1);
	void swapGun(bool flag);
	void reloadingGun();
	void setGun(Gun* gun);
	float getMoveSpeed();
	void takeItem(Itemble*, int cnt = 1);
	void heal();

	Gun* getGun();
	sf::Vector2f getDeltaShake();
	PlayerDef getPlayerDef();


	Sprite* sprite;
	Gun* kick;
	float pitch, posZ, maxEnergy, nowEnergy;
	float defence, nowStrenght, maxStrenght;
	int patrons;
	int money;
	int details;
	std::map<Itemble*, int> items;
private:
	bool isJump, jumpFlag;
	Map* nowMap;
	Item* maxHeal;
	sf::Vector2f shakeDelta;
	int nowGun;
	std::vector<Gun*> guns;
	float moveSpeed, nowSpeed, boostSpeed, shakeTime;

	void shakeCamera(float deltaTime, bool isRun);
	void gravity(float deltaTime);
	void takeMaxHeal();
};

#endif // !PLAYER
