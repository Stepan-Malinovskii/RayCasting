#include "UIManeger.h"

UIManager::UIManager(sf::RenderWindow* _window)
{
	window = window;
	init();
}

void UIManager::init()
{
	playerUI = [&](Player* player) 
		{
			if (!player->kick->isCanUsed())
			{
				player->kick->drawWeapon(window, { 0,0 });
			}
			else
			{
				auto gun = player->getGun();
				gun->drawWeapon(window, player->getDeltaShake());

				if (gun->resetFn != nullptr)
				{
					sf::Text weaponInfo(std::to_string(gun->nowCount) + " / " + std::to_string(gun->maxCountPotron), Resources::UIFont, 50);
					auto b = weaponInfo.getLocalBounds();
					weaponInfo.setOrigin({ b.width / 2, b.height / 2 });
					weaponInfo.setPosition({ SCREEN_W / 2, SCREEN_H / 2 - weaponInfo.getCharacterSize() / 4 });
					weaponInfo.setPosition({ SCREEN_W - b.width / 2 - 20, SCREEN_H - 45 });
					weaponInfo.setFillColor({ 0, 0, 0 });
					window->draw(weaponInfo);
				}
				float baseX = 300;
				sf::RectangleShape hpShape({ baseX, 20 });
				hpShape.setFillColor({ 128, 128, 128 });
				hpShape.setPosition({ 20, SCREEN_H - 55 });
				window->draw(hpShape);

				sf::RectangleShape boostShape{ hpShape };
				boostShape.move({ 0, 30 });
				window->draw(boostShape);

				hpShape.setFillColor({ 255, 23, 23 });
				float newXH = baseX * (player->sprite->spMap.nowHealPoint <= 0 ? 0 : 
					player->sprite->spMap.nowHealPoint) / player->sprite->spDef.maxHealpoint;
				hpShape.setSize({ newXH, 20 });
				window->draw(hpShape);

				boostShape.setFillColor({ 44, 148, 15 });
				float newXB = baseX * player->timerBoost / player->timeBoost;
				boostShape.setSize({ newXB, 20 });
				window->draw(boostShape);

				sf::CircleShape aim(1.0f, 16);
				aim.setFillColor(sf::Color::Black);
				aim.setPosition({ SCREEN_W / 2, SCREEN_H / 2 });
				window->draw(aim);
			}
		};
}

void UIManager::drawPlayerUI(Player* player)
{
	playerUI(player);
}