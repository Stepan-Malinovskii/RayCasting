#include "Player.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Windows.h>

Player::Player(Sprite* _sprite) : sprite{ _sprite }
{
	gunSprite.setSize((sf::Vector2f)Resources::playerGunTexture.getSize());
	gunSprite.setTexture(&Resources::playerGunTexture);
	gunSprite.setOrigin((sf::Vector2f)Resources::playerGunTexture.getSize());
	gunSprite.setPosition(1280, 720);

	lastMousePos = sf::Vector2i(640, 360);
}

void Player::UpdatePlayer(float deltaTime, Map& map, sf::RenderWindow& window)
{
	//MousePart
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	float deltaX = (mousePos.x - lastMousePos.x)/2.0f;
	sprite->angle += MOUSE_TURN_SPEED * ROTATION_SPEED * deltaX * deltaTime;
	sf::Mouse::setPosition(lastMousePos, window);

	//KeyBoardPart
	float radiansAngle = sprite->angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));
	sf::Vector2f horizontalMoveParametrs( - verticalMoveParametrs.y, verticalMoveParametrs.x); 
	sf::Vector2f deltaPos(0,0);

	if (GetAsyncKeyState('A'))
	{
		deltaPos -= horizontalMoveParametrs;
	}
	else if (GetAsyncKeyState('D'))
	{
		deltaPos += horizontalMoveParametrs;
	}
	if (GetAsyncKeyState('W'))
	{
		deltaPos += verticalMoveParametrs;
	}
	else if (GetAsyncKeyState('S'))
	{
		deltaPos -= verticalMoveParametrs;
	}

	sprite->move(map, deltaPos * MOVE_SPEED * deltaTime);
}

void Player::DrawPlayerUI(sf::RenderWindow& window)
{
	window.draw(gunSprite);
}