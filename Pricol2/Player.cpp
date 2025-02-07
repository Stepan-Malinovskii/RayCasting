#include "Player.h"
#include "Raycast.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Windows.h>

Player::Player(Sprite* _sprite) : sprite{ _sprite }
{
	if (!weaponBaseTexture.loadFromFile("Texture/weapon.png")) exit(2);
	if (!weaponTexture[0].loadFromFile("Texture/weapon_fire10.png")) exit(2);
	if (!weaponTexture[1].loadFromFile("Texture/weapon_fire11.png")) exit(2);
	if (!weaponTexture[2].loadFromFile("Texture/weapon_fire12.png")) exit(2);
	if (!weaponTexture[3].loadFromFile("Texture/weapon_fire13.png")) exit(2);

	weaponAnimator = Animator{ &weaponBaseTexture, {Animation<sf::Texture*>({
		{0.0f, &weaponTexture[0]},
		{0.15f, &weaponTexture[1]},
		{0.3f, &weaponTexture[2]},
		{0.45f, &weaponTexture[3]},
		{0.6f, &weaponTexture[2]},
		{0.75f, &weaponTexture[1]},
		{0.9f, &weaponTexture[0]},
		{1.0f, &weaponTexture[0]},

})}};
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
	static float fireTimer = 0.0f;
	fireTimer -= deltaTime;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && fireTimer <= 0.0f)
	{
		if (!justFired)
		{
			RayHit hit = raycast(map, sprite->position, verticalMoveParametrs, true, sprite);
			if (hit.sprite)
			{
				hit.sprite->healPoint -= damage;
			}
			//else if (hit.cell)
			//{
			//	//map.SetNewOnGrid(hit.mapPos.x, hit.mapPos.y, WALL_LAYER, 1); 
			//}
			fireTimer = 1.0f;
			justFired = true;
			weaponAnimator.setAnimation(0);
		}
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		justFired = false;
	}

	//AnimatorPart
	weaponAnimator.update(deltaTime);
}

void Player::DrawPlayerUI(sf::RenderWindow& window)
{
	sf::Texture* tex = weaponAnimator.get();
	if (tex)
	{
		sf::Sprite weapon{ *tex };
		weapon.setOrigin(tex->getSize().x / 2.0f, tex->getSize().y);
		weapon.setPosition(window.getSize().x / 2.0f, window.getSize().y);
		weapon.scale(2.5f, 2.5f);
		window.draw(weapon);
	}
}