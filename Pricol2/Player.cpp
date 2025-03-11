#include "Player.h"
#include "Raycast.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

Player::Player(Sprite* _sprite, Map* _nowMap) : sprite{ _sprite }, nowGun{ 1 }
{
	nowMap = _nowMap;
	pitch = 0;
	posZ = 0.0f;
	isJump = false;
	jumpFlag = false;
	moveSpeed = 5.0f, boostSpeed = 8.0f, timeBoost = 2.0f, timerBoost = timeBoost, nowSpeed = moveSpeed;
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

	Gun gun1 = Gun(50.0f, 5, 1.0f, 5.0f, 0.5f, shutAnim1, [&](Sprite* sp, float dist) {sp->healPoint -= 50.0f * (dist < 5.0f ? 1:0);}, [&](Gun* gun) {gun->nowCount++;});
	gun1.setSound(Resources::gun1ShutSound, Resources::gun1ResetSound, Resources::gun1CantShoutSound);
	guns.push_back(gun1);

	Gun gun2 = Gun(10.0f, 20, 0.5f, 10.0f, 3.0f, shutAnim2, [&](Sprite* sp, float dist) {sp->healPoint -= 10.0f * (dist < 10 ? 1 : 0);}, [&](Gun* gun) {gun->nowCount = 20;});
	gun2.setSound(Resources::gun2ShutSound, Resources::gun2ResetSound, Resources::gun2CantShoutSound);
	guns.push_back(gun2);
}

void Player::updateMouseData(sf::Vector2f mousePos, float deltaTime)
{
	//MousePart
	sprite->angle += MOUSE_TURN_SPEED * ROTATION_SPEED * mousePos.x * deltaTime;

	pitch -= mousePos.y * deltaTime * VERTICAL_MOUSE_SPEED;
	if (pitch > 200) pitch = 200;
	if (pitch < -200) pitch = -200;

	//AnimatorPart
	guns[nowGun].update(deltaTime);

	//GravityPart
	gravity(deltaTime);
}

void Player::checkBoost(bool isPressed, float deltaTime)
{
	static bool boostFlag = false, used = false;

	if (isPressed)
	{
		if (timerBoost > 0 && !boostFlag)
		{
			used = true;
			nowSpeed = boostSpeed;
			timerBoost -= deltaTime * 1.2f;
		}
		else if (timerBoost < 0)
		{
			boostFlag = true;
			used = false;
			nowSpeed = moveSpeed;
		}
	}
	else
	{
		nowSpeed = moveSpeed;
		used = false;
	}
	if (!used)
	{
		if (timerBoost < timeBoost)
		{
			timerBoost += deltaTime;
		}
		else
		{
			timerBoost = timeBoost;
			boostFlag = false;
		}
	}
}

void Player::move(sf::Vector2f deltaPos)
{
	sprite->move(*nowMap, deltaPos * nowSpeed);
}

void Player::gravity(float deltaTime)
{
	if (isJump)
	{
		if (posZ < 0)
		{
			isJump = false;
			jumpFlag = false;
			posZ = 0;
		}
		else
		{
			if (posZ > 150)
			{
				jumpFlag = true;
			}
			if (!jumpFlag)
			{
				posZ += 750 * deltaTime;
			}
			else
			{
				posZ -= 750 * deltaTime;
			}
			
			
		}
	}
	
}

void Player::jump()
{
	if (posZ == 0)
	{
		isJump = true;
	}
}

void Player::use()
{
	float radiansAngle = sprite->angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));
	RayHit hit = raycast(*nowMap, sprite->position, verticalMoveParametrs, true, sprite, 1);
	if (hit.sprite != nullptr)
	{
		hit.sprite->healPoint -= hit.sprite->healPoint;
	}
}

void Player::reloadingGun()
{
	guns[nowGun].resetPatron();
}

void Player::fire()
{
	float radiansAngle = sprite->angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));

	if (guns[nowGun].isCanUsed())
	{
		RayHit hit = raycast(*nowMap, sprite->position, verticalMoveParametrs, true, sprite, guns[nowGun].maxDist);
		float dist = 0;
		if (hit.sprite != nullptr) { dist = sqrt(GETDIST(hit.sprite->position, sprite->position)); }
		guns[nowGun].ussing(hit.sprite, dist);
	}
}

void Player::swapGun(bool flag)
{
	int delta = flag ? 1 : -1;
	nowGun++;
	nowGun = nowGun % guns.size();
}

void Player::DrawPlayerUI(sf::RenderWindow& window)
{
	auto gun = guns[nowGun];
	gun.drawWeapon(window);

	sf::Text weaponInfo(std::to_string(gun.nowCount) + " / " + std::to_string(gun.maxCountPotron), Resources::UIFont, 50);
	weaponInfo.setPosition({ SCREEN_W - 150, SCREEN_H - 60 });
	weaponInfo.setFillColor({ 0, 0, 0 });
	window.draw(weaponInfo);

	float baseX = 300;
	sf::RectangleShape hpShape({ baseX, 20 });
	hpShape.setFillColor({ 128, 128, 128 });
	hpShape.setPosition({ 20, SCREEN_H - 55 });
	sf::RectangleShape boostShape{ hpShape };
	boostShape.move({ 0, 30 });
	window.draw(hpShape);
	window.draw(boostShape);
	hpShape.setFillColor({ 255, 23, 23 });
	boostShape.setFillColor({ 44, 148, 15 });
	float newXB = baseX * timerBoost / timeBoost;
	float newXH = baseX * (sprite->healPoint <= 0 ? 0 : sprite->healPoint) / sprite->maxHealpoint;
	hpShape.setSize({ newXH, 20 });
	boostShape.setSize({ newXB, 20 });
	window.draw(hpShape);
	window.draw(boostShape);
}

Gun* Player::getWeapon() { return &guns[nowGun]; }

float Player::getMoveSpeed() { return moveSpeed; }