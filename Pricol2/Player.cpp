#include "Player.h"
#include "Raycast.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

Player::Player(Sprite* _sprite, Map* _nowMap) : sprite{ _sprite }, nowGun{ 0 }
{
	nowMap = _nowMap;
	pitch = 0, shakeTime = 0, posZ = 0.0f;
	isJump = false, jumpFlag = false;
	moveSpeed = 5.0f, boostSpeed = 8.0f, timeBoost = 2.0f, timerBoost = timeBoost, nowSpeed = moveSpeed;
}

void Player::setGun(Gun* gun) { guns.push_back(gun); }

void Player::updateMouseData(sf::Vector2f mousePos, float deltaTime)
{
	//MousePart
	sprite->spMap.angle += MOUSE_TURN_SPEED * ROTATION_SPEED * mousePos.x * deltaTime;

	pitch -= mousePos.y * deltaTime * VERTICAL_MOUSE_SPEED;
	if (pitch > 200) pitch = 200;
	if (pitch < -200) pitch = -200;

	//AnimatorPart
	guns[nowGun]->update(deltaTime);
	kick->update(deltaTime);

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

void Player::move(sf::Vector2f deltaPos, float deltaTime)
{
	sprite->move(nowMap, deltaPos * deltaTime * nowSpeed);
	shakeCamera(deltaTime, deltaPos != sf::Vector2f());
}

void Player::shakeCamera(float deltaTime, bool isRun)
{
	if (isRun)
	{
		float scale = 5.0f;
		shakeTime += deltaTime;
		shakeDelta.x = cosf(shakeTime * nowSpeed) * scale;
		shakeDelta.y = sinf(2 * shakeTime * nowSpeed) * scale;
	}
	else
	{
		shakeTime = 0;
		if (shakeDelta == sf::Vector2f()) return;

		shakeDelta *= 0.9f;

		if (abs(shakeDelta.x) < 0.0001f) shakeDelta.x = 0.0f;
		if (abs(shakeDelta.y) < 0.0001f) shakeDelta.y = 0.0f;
	}
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

void Player::reloadingGun()
{
	guns[nowGun]->resetPatron();
}

void Player::fire(int gun)
{
	float radiansAngle = sprite->spMap.angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));

	if (gun == -1)
	{
		if (kick->isCanUsed() && guns[nowGun]->isCanUsed())
		{
			RayHit hit = raycast(nowMap, sprite->spMap.position, verticalMoveParametrs, false, sprite, 1);
			if (hit.cell == 1) nowMap->SetNewOnGrid(hit.mapPos.x, hit.mapPos.y, WALL_LAYER, 0);
			kick->ussing(nullptr, 0);
		}
	}
	else
	{
		if (guns[nowGun]->isCanUsed() && kick->isCanUsed())
		{
			RayHit hit = raycast(nowMap, sprite->spMap.position, verticalMoveParametrs, true, sprite, guns[nowGun]->maxDist, pitch);
			float dist = 0;
			if (hit.sprite != nullptr) { dist = sqrt(GETDIST(hit.sprite->spMap.position, sprite->spMap.position)); }
			guns[nowGun]->ussing(hit.sprite, dist);
		}
	}
}

MapSprite* Player::dialog()
{
	float radiansAngle = sprite->spMap.angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));

	RayHit hit = raycast(nowMap, sprite->spMap.position, verticalMoveParametrs, true, sprite, 1, pitch);
	if (hit.sprite != nullptr && hit.sprite->spDef.type == SpriteType::NPC) { return &hit.sprite->spMap; }
	return nullptr;
}

void Player::swapGun(bool flag)
{
	int delta = flag ? 1 : -1;
	nowGun += delta;
	nowGun = nowGun < 0 ? nowGun = guns.size() - 1 : nowGun % guns.size();
}

void Player::swapMap(Map* newMap) { nowMap = newMap; }

Gun* Player::getGun() { return guns[nowGun]; }

sf::Vector2f Player::getDeltaShake() { return shakeDelta; }

float Player::getMoveSpeed() { return moveSpeed; }