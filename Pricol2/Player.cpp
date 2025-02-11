#include "Player.h"
#include "Raycast.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Windows.h>

Player::Player(Sprite* _sprite) : sprite{ _sprite }, nowGun{ 1 }
{
	Animator shutAnim1 = Animator<sf::Texture*>{ &Resources::gun1BaseTexture, {Animation<sf::Texture*>({
		{0.0f, &Resources::gun1FireAnimationTexture[0]},
		{0.15f, &Resources::gun1FireAnimationTexture[1]},
		{0.3f, &Resources::gun1FireAnimationTexture[2]},
		{0.45f, &Resources::gun1FireAnimationTexture[3]},
		{0.6f, &Resources::gun1FireAnimationTexture[2]},
		{0.75f, &Resources::gun1FireAnimationTexture[1]},
		{0.9f, &Resources::gun1FireAnimationTexture[0]},
		{1.0f, &Resources::gun1FireAnimationTexture[0]}})} };

	Animator shutAnim2 = Animator<sf::Texture*>{ &Resources::gun2BaseTexture, {Animation<sf::Texture*>({
		{0.0f, &Resources::gun2FireAnimationTexture[0]},
		{0.071f, &Resources::gun2FireAnimationTexture[1]},
		{0.142f, &Resources::gun2FireAnimationTexture[2]},
		{0.214f, &Resources::gun2FireAnimationTexture[3]},
		{0.285f, &Resources::gun2FireAnimationTexture[2]},
		{0.357f, &Resources::gun2FireAnimationTexture[1]},
		{0.428f, &Resources::gun2FireAnimationTexture[0]},
		{0.5f, &Resources::gun2FireAnimationTexture[0]}})} };

	Gun gun1 = Gun(50.0f, 5, 1.0f, 0.5f, shutAnim1, [&](Sprite* sp) {sp->healPoint -= 50.0f;}, [&](Gun* gun) {gun->nowCount++;});
	gun1.setSound(Resources::gun1ShutSound, Resources::gun1ResetSound, Resources::gun1CantShoutSound);
	guns.push_back(gun1);

	Gun gun2 = Gun(10.0f, 20, 0.5f, 3.0f, shutAnim2, [&](Sprite* sp) {sp->healPoint -= 10.0f;}, [&](Gun* gun) {gun->nowCount = 20;});
	gun2.setSound(Resources::gun2ShutSound, Resources::gun2ResetSound, Resources::gun2CantShoutSound);
	guns.push_back(gun2);

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

	if (GetAsyncKeyState('R'))
	{
		guns[nowGun].resetPatron();
	}

	//MousePart
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	float deltaX = (mousePos.x - lastMousePos.x)/2.0f;
	sprite->angle += MOUSE_TURN_SPEED * ROTATION_SPEED * deltaX * deltaTime;
	sf::Mouse::setPosition(lastMousePos, window);

	static bool justFired = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && guns[nowGun].isCanUsed())
	{
		if (!justFired)
		{
			RayHit hit = raycast(map, sprite->position, verticalMoveParametrs, true, sprite);
			guns[nowGun].ussing(hit.sprite);
			justFired = true;
		}
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		justFired = false;
	}

	static bool justSwitch = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && !justSwitch)
	{
		nowGun++;
		nowGun = nowGun % guns.size();
		justSwitch = true;
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		justSwitch = false;
	}

	//AnimatorPart
	guns[nowGun].update(deltaTime);
}

void Player::DrawPlayerUI(sf::RenderWindow& window)
{
	guns[nowGun].drawWeapon(window);
}