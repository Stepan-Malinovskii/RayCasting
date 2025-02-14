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

constexpr float ROTATION_SPEED = 80.0f, MOUSE_TURN_SPEED = 0.08f;

class Player
{
public:
	Player(Sprite* sprite);

	void UpdatePlayer(float deltaTimeá, Map& map, sf::RenderWindow& window);
	void DrawPlayerUI(sf::RenderWindow& window);

	Gun* getWeapon();
	float getMoveSpeed();
	Sprite* sprite;
	float timeBoost, timerBoost;
private:
	sf::Vector2i lastMousePos;
	int nowGun;
	float moveSpeed, nowSpeed, boostSpeed;
	std::vector<Gun> guns;
};

#endif // !PLAYER
