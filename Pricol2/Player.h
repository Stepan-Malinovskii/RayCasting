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

class Player
{
public:
	Player(Sprite* sprite, Map* _nowMap = nullptr);

	void updateMouseData(sf::Vector2f mousePos, float deltaTime);
	void checkBoost(bool isPressed, float deltaTime);
	void move(sf::Vector2f deltaPos, float deltaTime);
	void jump();
	void fire(int gun = -1);
	void swapGun(bool flag);
	void reloadingGun();
	void swapMap(Map* newMap);
	void DrawPlayerUI(sf::RenderWindow* window);
	void setGun(Gun* gun);
	float getMoveSpeed();

	Sprite* sprite;
	float pitch, posZ;
	Gun* kick;
private:
	bool isJump, jumpFlag;
	Map* nowMap;
	sf::Vector2f shakeDelta;
	int nowGun;
	std::vector<Gun*> guns;
	float moveSpeed, nowSpeed, boostSpeed,
		timeBoost, timerBoost, shakeTime;

	void shakeCamera(float deltaTime, bool isRun);
	void gravity(float deltaTime);
};

#endif // !PLAYER
