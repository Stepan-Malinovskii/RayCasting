#pragma once
#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

#include "Map.h"
#include "Resources.h"
#include "Animation.h"
#include "Weapon.h"

constexpr float MOVE_SPEED = 4.0f, ROTATION_SPEED = 80.0f, MOUSE_TURN_SPEED = 0.08f;

class Player
{
public:
	Player(Sprite* sprite);

	void UpdatePlayer(float deltaTimeá, Map& map, sf::RenderWindow& window);
	void DrawPlayerUI(sf::RenderWindow& window);

	Sprite* sprite;
private:
	sf::Vector2i lastMousePos;
	Gun gun;
};

#endif // !PLAYER
