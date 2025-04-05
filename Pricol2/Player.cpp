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
	sprite->spMap.angle += MOUSE_TURN_SPEED * ROTATION_SPEED * mousePos.x * deltaTime;

	pitch = std::clamp(pitch - mousePos.y * deltaTime * VERTICAL_MOUSE_SPEED, -200.0f, 200.0f);

	guns[nowGun]->update(deltaTime);
	if (kick) kick->update(deltaTime);

	gravity(deltaTime);
}

void Player::checkBoost(bool isPressed, float deltaTime)
{
	static bool boostFlag = false;

	nowSpeed = sprite->spDef.speed;

	if (boostFlag) {
		nowEnergy = std::min(nowEnergy + deltaTime, maxEnergy);
		if (nowEnergy >= maxEnergy) {
			boostFlag = false;
		}
		return;
	}

	if (!isPressed) {
		nowEnergy = std::min(nowEnergy + deltaTime, maxEnergy);
	}
	else {
		if (nowEnergy > 0) {
			nowSpeed = boostSpeed;
			nowEnergy = std::max(nowEnergy - deltaTime * 1.2f, 0.0f);
		}
		else {
			boostFlag = true;
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

		shakeDelta.x = std::abs(shakeDelta.x) < 0.0001f ? 0.0f : shakeDelta.x;
		shakeDelta.y = std::abs(shakeDelta.y) < 0.0001f ? 0.0f : shakeDelta.y;
	}
}

void Player::gravity(float deltaTime)
{
	if (!isJump) return;
	
	if (posZ < 0)
	{
		isJump = false;
		jumpFlag = false;
		posZ = 0;
	}
	else
	{
		if (posZ > 150) jumpFlag = true;

		posZ += (jumpFlag ? -750 : 750) * deltaTime;
		posZ = std::max(posZ, -1.0f);
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
	sf::Vector2f direction(cos(radiansAngle), sin(radiansAngle));

	if (gun == -1)
	{
		if (kick->isCanUsed() && guns[nowGun]->isCanUsed())
		{
			RayHit hit = raycast(nowMap, sprite->spMap.position, direction, false, sprite, 1);
			if (hit.cell == 1) nowMap->SetNewOnGrid(hit.mapPos.x, hit.mapPos.y, WALL_LAYER, 0);
			kick->ussing(nullptr, 0);
		}
	}
	else
	{
		if (guns[nowGun]->isCanUsed() && kick->isCanUsed())
		{
			RayHit hit = raycast(nowMap, sprite->spMap.position, direction, true, sprite, guns[nowGun]->maxDist, pitch);
			float dist = hit.sprite && hit.sprite->spDef.type != NPC ? dist = sqrt(GETDIST(hit.sprite->spMap.position, sprite->spMap.position)): 0; 
			guns[nowGun]->ussing(hit.sprite, dist);
		}
	}
}

Sprite* Player::dialog()
{
	float radiansAngle = sprite->spMap.angle * PI / 180.0f;
	sf::Vector2f direction(cos(radiansAngle), sin(radiansAngle));

	RayHit hit = raycast(nowMap, sprite->spMap.position, direction, true, sprite, 1, pitch);
	return hit.sprite && hit.sprite->spDef.type == SpriteType::NPC ? hit.sprite : nullptr;
}

void Player::swapGun(bool flag)
{
	int delta = flag ? 1 : -1;
	nowGun = (nowGun + delta + 3) % 3;
	while (!guns[nowGun]) 
	{
		nowGun = (nowGun + delta + 3) % 3;
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

void Player::setNemMap(Map* _map) { nowMap = _map; }

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
	if (nowHeal)
	{
		nowHeal->useFunc(this);
		invent->useItem(nowHeal);
		nowHeal = invent->takeMaxHeal();
	}
}