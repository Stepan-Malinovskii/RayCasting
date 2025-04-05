#include "Player.h"
#include "Raycast.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Inventory.h"

Player::Player(Sprite* _sprite, PlayerDef def, Map* _nowMap) : 
	sprite{ _sprite }, nowGun{ 0 }, nowEnergy{ def.nowEnergy }, maxEnergy { def.maxEnergy },
	defence{ def.defence}, nowStrenght{ def.nowStrenght }, maxStrenght{ def.maxStrenght }, 
	patrons{ def.countpantrons }, nowMap{ _nowMap }, money{ def.money }, details{ def.details }
{
	nowHeal = nullptr;
	kick = nullptr;
	pitch = 0, shakeTime = 0, posZ = 0.0f;
	isJump = false, jumpFlag = false;
	boostSpeed = 8.0f, nowSpeed = sprite->spDef.speed;
}

Gun* Player::setGun(Gun* gun, int pos) 
{
	auto temp = guns[pos];
	guns[pos] = gun;
	return temp;
}

Gun* Player::getGun(int pos)
{
	return guns[pos];
}

void Player::setInventory(Inventory* _invent) 
{ 
	invent = _invent; 
	nowHeal = invent->takeMaxHeal();
}

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
		if (nowEnergy > 0 && !boostFlag)
		{
			used = true;
			nowSpeed = boostSpeed;
			nowEnergy -= deltaTime * 1.2f;
		}
		else if (nowEnergy < 0)
		{
			boostFlag = true;
			used = false;
			nowSpeed = sprite->spDef.speed;
		}
	}
	else
	{
		nowSpeed = sprite->spDef.speed;
		used = false;
	}
	if (!used)
	{
		if (nowEnergy < maxEnergy)
		{
			nowEnergy += deltaTime;
		}
		else
		{
			nowEnergy = maxEnergy;
			boostFlag = false;
		}
	}
}

void Player::move(sf::Vector2f deltaPos, float deltaTime)
{
	guns[nowGun]->updateRad(deltaPos != sf::Vector2f(), deltaTime);
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
	patrons = guns[nowGun]->resetPatron(patrons);
}

void Player::takeDamage(float damage)
{
	if (nowStrenght > 0)
	{
		damage *= (1 - defence);
		nowStrenght -= damage * defence;

		if (nowStrenght <= 0)
		{
			nowStrenght = 0;
			maxStrenght = 0;
			defence = 0;
		}
	}

	sprite->takeDamage(damage);
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

Sprite* Player::dialog()
{
	float radiansAngle = sprite->spMap.angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));

	RayHit hit = raycast(nowMap, sprite->spMap.position, verticalMoveParametrs, true, sprite, 1, pitch);
	if (hit.sprite != nullptr && hit.sprite->spDef.type == SpriteType::NPC) { return hit.sprite; }
	return nullptr;
}

void Player::swapGun(bool flag)
{
	int delta = flag ? 1 : -1;
	nowGun += delta;
	nowGun = nowGun < 0 ? nowGun = 2 : nowGun % 3;
	while (guns[nowGun] == nullptr)
	{
		nowGun += delta;
		nowGun = nowGun < 0 ? nowGun = 2 : nowGun % 3;
	}
	
}

PlayerDef Player::getPlayerDef()
{
	std::vector<int> gunsData;

	for (auto it : guns)
	{
		if (it != nullptr && it->name != L"Кулак")
		{
			gunsData.push_back(it->id);
		}
	}

	return { sprite->spDef.maxHealpoint,
	sprite->spMap.nowHealPoint,
	maxEnergy,
	nowEnergy,
	defence,
	maxStrenght,
	nowStrenght,
	patrons,
	money,
	details,
	gunsData };
}

Gun* Player::getNowGun() { return guns[nowGun]; }

sf::Vector2f Player::getDeltaShake() { return shakeDelta; }

float Player::getMoveSpeed() { return nowSpeed; }

void Player::takeItem(Itemble* item, int cnt)
{
	invent->takeItem(item, cnt);
	nowHeal = invent->takeMaxHeal();
}

void Player::heal()
{
	if (nowHeal != nullptr)
	{
		nowHeal->useFunc(this);
		invent->useItem(nowHeal);

		nowHeal = invent->takeMaxHeal();
	}
}