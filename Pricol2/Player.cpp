#include "Player.h"
#include "Raycast.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Windows.h>

Player::Player(Sprite* _sprite) : sprite{ _sprite }
{
	gun = Gun(10.0f, 1.0f, { &Resources::gun1BaseTexture, {Animation<sf::Texture*>({
		{0.0f, &Resources::gun1FireAnimationTexture[0]},
		{0.15f, &Resources::gun1FireAnimationTexture[1]},
		{0.3f, &Resources::gun1FireAnimationTexture[2]},
		{0.45f, &Resources::gun1FireAnimationTexture[3]},
		{0.6f, &Resources::gun1FireAnimationTexture[2]},
		{0.75f, &Resources::gun1FireAnimationTexture[1]},
		{0.9f, &Resources::gun1FireAnimationTexture[0]},
		{1.0f, &Resources::gun1FireAnimationTexture[0]},
		})} },[=](Sprite* sp) {sp->healPoint -= 10.0f;});
	lastMousePos = sf::Vector2i(640, 360);
}

void Player::UpdatePlayer(float deltaTime, Map& map, sf::RenderWindow& window)
{
	//KeyBoardPart
	float radiansAngle = sprite->angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));
	sf::Vector2f horizontalMoveParametrs(-verticalMoveParametrs.y, verticalMoveParametrs.x);
	sf::Vector2f deltaPos(0, 0);

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

	//MousePart
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	float deltaX = (mousePos.x - lastMousePos.x)/2.0f;
	sprite->angle += MOUSE_TURN_SPEED * ROTATION_SPEED * deltaX * deltaTime;
	sf::Mouse::setPosition(lastMousePos, window);

	static bool justFired = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && gun.isCanUsed())
	{
		if (!justFired)
		{
			RayHit hit = raycast(map, sprite->position, verticalMoveParametrs, true, sprite);
			if (hit.sprite)
			{
				gun.ussing(hit.sprite);
			}
			//else if (hit.cell)
			//{
			//	//map.SetNewOnGrid(hit.mapPos.x, hit.mapPos.y, WALL_LAYER, 1); 
			//}
			justFired = true;
			gun.startAnimation(0);
		}
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		justFired = false;
	}

	//AnimatorPart
	gun.update(deltaTime);
}

void Player::DrawPlayerUI(sf::RenderWindow& window)
{
	gun.drawWeapon(window);
}