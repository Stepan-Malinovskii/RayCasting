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

	Animation< sf::Texture* > shutAnim1({
		{0.0f, &Resources::gun1FireAnimationTexture[0]},
		{0.15f, &Resources::gun1FireAnimationTexture[1]},
		{0.3f, &Resources::gun1FireAnimationTexture[2]},
		{0.45f, &Resources::gun1FireAnimationTexture[3]},
		{0.6f, &Resources::gun1FireAnimationTexture[2]},
		{0.75f, &Resources::gun1FireAnimationTexture[1]},
		{0.9f, &Resources::gun1FireAnimationTexture[0]},
		{1.0f, &Resources::gun1FireAnimationTexture[0]} });
	Animation<sf::Texture*> resetAnim1({
	{0.000f, &Resources::gun1ResetAnimationTexture[0]},
	{0.166f, &Resources::gun1ResetAnimationTexture[1]},
	{0.332f, &Resources::gun1ResetAnimationTexture[2]},
	{0.498f, &Resources::gun1ResetAnimationTexture[3]},
	{0.664f, &Resources::gun1ResetAnimationTexture[4]},
	{0.830f, &Resources::gun1ResetAnimationTexture[5]},
	{0.996f, &Resources::gun1ResetAnimationTexture[6]},
	{1.162f, &Resources::gun1ResetAnimationTexture[1]},
	{1.328f, &Resources::gun1ResetAnimationTexture[0]},
	{1.500f, &Resources::gun1ResetAnimationTexture[0]} });
	Animator animr1 = Animator<sf::Texture*>{ &Resources::gun1BaseTexture, {shutAnim1, resetAnim1} };
	Gun gun1 = Gun(50.0f, 5, 1.0f, 5.0f, 1.5f);
	gun1.setAnimator(animr1);
	gun1.setResetFunc([&](Gun* gun) {gun->nowCount++;});
	gun1.setShutFunc([&](Sprite* sp, float dist) {sp->healPoint -= 50.0f * (dist < 5.0f ? 1:0);});
	gun1.setSound(Resources::gun1ShutSound, Resources::gun1ResetSound, Resources::gun1CantShoutSound);
	guns.push_back(gun1);

	Animation< sf::Texture* > shutAnim2({
		{0.066f, &Resources::gun2FireAnimationTexture[0]},
		{0.132f, &Resources::gun2FireAnimationTexture[1]},
		{0.198f, &Resources::gun2FireAnimationTexture[2]},
		{0.264f, &Resources::gun2FireAnimationTexture[1]},
		{0.330f, &Resources::gun2FireAnimationTexture[0]},
		{0.400f, &Resources::gun2FireAnimationTexture[0]} });
	Animation<sf::Texture*> resetAnim2({
	{0.000f, &Resources::gun2ResetAnimationTexture[0]},
	{0.214f, &Resources::gun2ResetAnimationTexture[1]},
	{0.428f, &Resources::gun2ResetAnimationTexture[2]},
	{0.642f, &Resources::gun2ResetAnimationTexture[3]},
	{0.856f, &Resources::gun2ResetAnimationTexture[4]},
	{1.070f, &Resources::gun2ResetAnimationTexture[5]},
	{1.284f, &Resources::gun2ResetAnimationTexture[6]},
	{1.500f, &Resources::gun2ResetAnimationTexture[6]} });
	Animator animr2 = Animator<sf::Texture*>{ &Resources::gun2BaseTexture, {shutAnim2, resetAnim2} };
	Gun gun2 = Gun(10.0f, 20, 0.4f, 20.0f, 1.5f);
	gun2.setAnimator(animr2);
	gun2.setResetFunc([&](Gun* gun) {gun->nowCount = 20;});
	gun2.setShutFunc([&](Sprite* sp, float dist) {sp->healPoint -= 10.0f * (dist < 20 ? 1 : 0);});
	gun2.setSound(Resources::gun2ShutSound, Resources::gun2ResetSound, Resources::gun1CantShoutSound);
	guns.push_back(gun2);

	Animation<sf::Texture*> shutAnim3({ 
		{0.0f, &Resources::gun3FireAnimationTexture[0]},
		{0.1f, &Resources::gun3FireAnimationTexture[1]},
		{0.2f, &Resources::gun3FireAnimationTexture[2]},
		{0.3f, &Resources::gun3FireAnimationTexture[1]},
		{0.4f, &Resources::gun3FireAnimationTexture[0]},
		{0.5f, &Resources::gun3FireAnimationTexture[0]} });
	Animation<sf::Texture*> resetAnim3({
		{0.167f, &Resources::gun3ResetAnimationTexture[0]},
		{0.334f, &Resources::gun3ResetAnimationTexture[1]},
		{0.501f, &Resources::gun3ResetAnimationTexture[2]},
		{0.668f, &Resources::gun3ResetAnimationTexture[3]},
		{0.835f, &Resources::gun3ResetAnimationTexture[4]},
		{1.000f, &Resources::gun3ResetAnimationTexture[5]},
		{1.169f, &Resources::gun3ResetAnimationTexture[6]},
		{1.336f, &Resources::gun3ResetAnimationTexture[7]},
		{1.503f, &Resources::gun3ResetAnimationTexture[8]},
		{1.670f, &Resources::gun3ResetAnimationTexture[9]},
		{1.837f, &Resources::gun3ResetAnimationTexture[10]},
		{2.000f, &Resources::gun3ResetAnimationTexture[11]},
		{2.171f, &Resources::gun3ResetAnimationTexture[12]},
		{2.338f, &Resources::gun3ResetAnimationTexture[13]},
		{2.505f, &Resources::gun3ResetAnimationTexture[14]},
		{2.672f, &Resources::gun3ResetAnimationTexture[15]},
		{2.839f, &Resources::gun3ResetAnimationTexture[16]},
		{3.0f, &Resources::gun3ResetAnimationTexture[0]}});
	Animator animr3 = Animator<sf::Texture*>{ &Resources::gun3BaseTexture, {shutAnim3, resetAnim3} };
	Gun gun3 = Gun(100.0f, 2, 0.6f, 5.0f, 3.0f);
	gun3.setAnimator(animr3);
	gun3.setResetFunc([&](Gun* gun) {gun->nowCount = 2;});
	gun3.setShutFunc([&](Sprite* sp, float dist) {sp->healPoint -= 100.0f * (dist < 5 ? 1 : 0);});
	gun3.setSound(Resources::gun3ShutSound, Resources::gun3ResetSound, Resources::gun1CantShoutSound);
	guns.push_back(gun3);

	Animation<sf::Texture*> shutAnim4({
		{0.005f, &Resources::gun4FireAnimationTexture[0]},
		{0.010f, &Resources::gun4FireAnimationTexture[1]},
		{0.015f, &Resources::gun4FireAnimationTexture[2]},
		{0.020f, &Resources::gun4FireAnimationTexture[2]},
		{0.025f, &Resources::gun4FireAnimationTexture[1]},
		{0.030f, &Resources::gun4FireAnimationTexture[0]},
		});
	Animation<sf::Texture*> resetAnim4({
		{0.25f, &Resources::gun4ResetAnimationTexture[0]},
		{0.50f, &Resources::gun4ResetAnimationTexture[1]},
		{0.75f, &Resources::gun4ResetAnimationTexture[2]},
		{1.00f, &Resources::gun4ResetAnimationTexture[3]},
		{1.25f, &Resources::gun4ResetAnimationTexture[4]},
		{1.50f, &Resources::gun4ResetAnimationTexture[5]},
		{1.75f, &Resources::gun4ResetAnimationTexture[6]},
		{2.00f, &Resources::gun4ResetAnimationTexture[7]},
		{2.25f, &Resources::gun4ResetAnimationTexture[8]},
		{2.50f, &Resources::gun4ResetAnimationTexture[9]},
		{2.75f, &Resources::gun4ResetAnimationTexture[10]},
		{2.85f, &Resources::gun4ResetAnimationTexture[8]},
		{3.0f, &Resources::gun4ResetAnimationTexture[8]} });
	Animator animr4 = Animator<sf::Texture*>{ &Resources::gun4BaseTexture, {shutAnim4, resetAnim4} };
	Gun gun4 = Gun(15.0f, 30, 0.3f, 15.0f, 3.0f);
	gun4.setAnimator(animr4);
	gun4.setResetFunc([&](Gun* gun) {gun->nowCount = 30;});
	gun4.setShutFunc([&](Sprite* sp, float dist) {sp->healPoint -= 15.0f * (dist < 15 ? 1 : 0);});
	gun4.setSound(Resources::gun4ShutSound, Resources::gun4ResetSound, Resources::gun1CantShoutSound);
	guns.push_back(gun4);

	Animation<sf::Texture*> shutAnim5({
		{0.0f, &Resources::gun5FireAnimationTexture[0]},
		{0.2f, &Resources::gun5FireAnimationTexture[1]},
		{0.4f, &Resources::gun5FireAnimationTexture[2]},
		{0.6f, &Resources::gun5FireAnimationTexture[1]},
		{0.8f, &Resources::gun5FireAnimationTexture[0]},
		{1.0f, &Resources::gun5FireAnimationTexture[0]} });
	Animation<sf::Texture*> resetAnim5({
		{0.21f, &Resources::gun5ResetAnimationTexture[0]},
		{0.42f, &Resources::gun5ResetAnimationTexture[1]},
		{0.63f, &Resources::gun5ResetAnimationTexture[2]},
		{0.84f, &Resources::gun5ResetAnimationTexture[3]},
		{1.05f, &Resources::gun5ResetAnimationTexture[4]},
		{1.26f, &Resources::gun5ResetAnimationTexture[5]},
		{1.47f, &Resources::gun5ResetAnimationTexture[6]},
		{1.68f, &Resources::gun5ResetAnimationTexture[7]},
		{1.89f, &Resources::gun5ResetAnimationTexture[8]},
		{2.10f, &Resources::gun5ResetAnimationTexture[9]},
		{2.31f, &Resources::gun5ResetAnimationTexture[10]},
		{2.52f, &Resources::gun5ResetAnimationTexture[11]},
		{2.73f, &Resources::gun5ResetAnimationTexture[12]},
		{3.00f, &Resources::gun5ResetAnimationTexture[1]} });
	Animator animr5 = Animator<sf::Texture*>{ &Resources::gun5BaseTexture, {shutAnim5, resetAnim5} };
	Gun gun5 = Gun(200.0f, 1, 1.0f, 10.0f, 3.0f);
	gun5.setAnimator(animr5);
	gun5.setResetFunc([&](Gun* gun) {gun->nowCount = 1;});
	gun5.setShutFunc([&](Sprite* sp, float dist) {sp->healPoint -= 200.0f * (dist < 10 ? 1 : 0);});
	gun5.setSound(Resources::gun5ShutSound, Resources::gun5ResetSound, Resources::gun1CantShoutSound);
	guns.push_back(gun5);
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

void Player::use()
{
	float radiansAngle = sprite->angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));
	RayHit hit = raycast(nowMap, sprite->position, verticalMoveParametrs, true, sprite, 1);
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
		RayHit hit = raycast(nowMap, sprite->position, verticalMoveParametrs, true, sprite, guns[nowGun].maxDist, pitch);
		float dist = 0;
		if (hit.sprite != nullptr) { dist = sqrt(GETDIST(hit.sprite->position, sprite->position)); }
		guns[nowGun].ussing(hit.sprite, dist);
	}
}

void Player::swapGun(bool flag)
{
	int delta = flag ? 1 : -1;
	nowGun += delta;
	nowGun = nowGun < 0 ? nowGun = guns.size() - 1 : nowGun % guns.size();
}

void Player::DrawPlayerUI(sf::RenderWindow* window)
{
	auto gun = guns[nowGun];
	gun.drawWeapon(window, shakeDelta);

	sf::Text weaponInfo(std::to_string(guns[nowGun].nowCount) + " / " + std::to_string(guns[nowGun].maxCountPotron), Resources::UIFont, 50);
	weaponInfo.setPosition({ SCREEN_W - 150, SCREEN_H - 60 });
	weaponInfo.setFillColor({ 0, 0, 0 });
	window->draw(weaponInfo);

	float baseX = 300;
	sf::RectangleShape hpShape({ baseX, 20 });
	hpShape.setFillColor({ 128, 128, 128 });
	hpShape.setPosition({ 20, SCREEN_H - 55 });
	window->draw(hpShape);

	sf::RectangleShape boostShape{ hpShape };
	boostShape.move({ 0, 30 });
	window->draw(boostShape);

	hpShape.setFillColor({ 255, 23, 23 });
	float newXH = baseX * (sprite->healPoint <= 0 ? 0 : sprite->healPoint) / sprite->maxHealpoint;
	hpShape.setSize({ newXH, 20 });
	window->draw(hpShape);

	boostShape.setFillColor({ 44, 148, 15 });
	float newXB = baseX * timerBoost / timeBoost;
	boostShape.setSize({ newXB, 20 });
	window->draw(boostShape);

	sf::CircleShape aim(1.0f, 16);
	aim.setFillColor(sf::Color::Black);
	aim.setPosition({SCREEN_W / 2, SCREEN_H / 2});
	window->draw(aim);
}

void Player::swapMap(Map* newMap) { nowMap = newMap; }

Gun* Player::getWeapon() { return &guns[nowGun]; }

float Player::getMoveSpeed() { return moveSpeed; }