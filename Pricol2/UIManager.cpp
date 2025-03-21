#include "UIManeger.h"

UIManager::UIManager(sf::RenderWindow* _window)
{
	window = _window;
	initPlayer();
}

void UIManager::initDialog(std::vector<std::pair<std::wstring, int>> variants, 
	std::wstring npcName)
{
	buttons.clear();
	sf::RectangleShape nameBase{ {DIALOG_W, TEXTSIZE + 10} };
	nameBase.setFillColor(sf::Color(100, 100, 100));
	sf::Text nameText(npcName, Resources::UIFont, TEXTSIZE);
	Group g(nameBase, nameText);
	g.setPosition({ SCREEN_W / 2, INTERVAL});
	buttons.push_back(DialogButton(g));

	sf::RectangleShape dataBase{ {DIALOG_W, DIALOG_H / 2.5f} };
	dataBase.setFillColor(sf::Color(100, 100, 100));
	sf::Text dataText(variants[0].first, Resources::UIFont, TEXTSIZE - 10);
	Group g1(dataBase, dataText);
	g1.setPosition({ SCREEN_W / 2, g.getPosition().y + g1.getSize().y / 2 + INTERVAL});
	buttons.push_back(DialogButton(g1));

	sf::Vector2f pos(SCREEN_W / 2, g1.getPosition().y + g1.getSize().y / 2 + INTERVAL);

	int i = 1;
	for (; i < variants.size(); i++)
	{
		sf::Text text(variants[i].first, Resources::UIFont, TEXTSIZE);
		sf::RectangleShape rect({ DIALOG_W, TEXTSIZE + 10 });
		rect.setFillColor(sf::Color(50, 50, 50));
		Group g2(rect, text);
		g2.setPosition(pos);
		
		buttons.push_back(DialogButton(g2));
		buttons.back().setFunc([=]() { keyButton = variants[i].second;});

		pos.y +=  INTERVAL + g2.shape.getSize().y / 2;
	}
}

void UIManager::deleteDialog() { buttons.clear(); }

int UIManager::checkButton(sf::Vector2i mousePos)
{
	for (auto b : buttons)
	{
		if (b.isClicked(mousePos))
		{
			b.update();
			return keyButton;
		}
	}
	return -1;
}

void UIManager::drawDialog()
{
	sf::Sprite back(Resources::dialogBackround);
	back.setScale({ SCREEN_W / Resources::dialogBackround.getSize().x,
		SCREEN_H / Resources::dialogBackround.getSize().y });
	window->draw(back);
	for (auto b : buttons)
	{
		b.drawButton(window);
	}
}

void UIManager::initPlayer()
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