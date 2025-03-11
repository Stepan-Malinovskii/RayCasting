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
	void move(sf::Vector2f deltaPos);
	void jump();
	void fire();
	void use();
	void swapGun(bool flag);
	void reloadingGun();
	void DrawPlayerUI(sf::RenderWindow& window);

	Gun* getWeapon();
	float getMoveSpeed();
	Sprite* sprite;
	Map* nowMap;
	float timeBoost, timerBoost, pitch, posZ;
private:
	bool isJump, jumpFlag;
	void gravity(float deltaTime);
	int nowGun;
	float moveSpeed, nowSpeed, boostSpeed;
	std::vector<Gun> guns;
};

#endif // !PLAYER
