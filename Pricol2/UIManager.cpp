#include "UIManeger.h"
#include <sstream>
#include <iomanip>

UIManager::UIManager(sf::RenderWindow* _window)
{
	window = _window;
	initPlayer();
}

std::wstring UIManager::splitText(std::wstring text, int maxLen, int textSize)
{
	std::wstring result, word, curLine;
	std::wistringstream stream(text);

	while (stream >> word)
	{
		sf::Text tempText;
		tempText.setFont(Resources::UIFont);
		tempText.setCharacterSize(textSize);
		std::wstring testLine = curLine.empty() ? word : curLine + L" " + word;
		tempText.setString(testLine);

		if (tempText.getLocalBounds().width > maxLen)
		{
			result += curLine + L"\n";
			curLine = word;
		}
		else
		{
			curLine = testLine;
		}
	}

	if (!curLine.empty())
	{
		result += curLine;
	}

	return result;
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
	sf::Text dataText(splitText(variants[0].first, DIALOG_W, 40), Resources::UIFont, TEXTSIZE - 10);
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

				if (gun->isReset)
				{
					sf::Text weaponInfo(std::to_string(gun->nowCount) + " / " + std::to_string(gun->maxCount), Resources::UIFont, 50);
					auto b = weaponInfo.getLocalBounds();
					weaponInfo.setOrigin({ b.width / 2, b.height / 2 });
					weaponInfo.setPosition({ SCREEN_W / 2, SCREEN_H / 2 - weaponInfo.getCharacterSize() / 4 });
					weaponInfo.setPosition({ SCREEN_W - b.width / 2 - 20, SCREEN_H - 45 });
					weaponInfo.setFillColor({ 0, 0, 0 });
					window->draw(weaponInfo);
				}
			}
			float baseX = 300;
			sf::RectangleShape baseShape({ baseX, 40 });
			baseShape.setFillColor({ 128, 128, 128 });
			sf::Text text("", Resources::UIFont, 30);
			Group group1({baseShape, text});
			group1.setPosition({ 170, SCREEN_H - 80 });
			window->draw(group1.shape);

			Group group2(group1);
			group2.setPosition({ group2.getPosition().x, group2.getPosition().y + 40});
			window->draw(group2.shape);

			std::ostringstream oss;
			oss << std::fixed << std::setprecision(2) << player->sprite->spMap.nowHealPoint;
			oss << " / ";
			oss << std::fixed << std::setprecision(2) << player->sprite->spDef.maxHealpoint;
			std::string str = oss.str();
			
			group1.shape.setFillColor({ 255, 23, 23 });
			float newXH = baseX * (player->sprite->spMap.nowHealPoint <= 0 ? 0 :
				player->sprite->spMap.nowHealPoint) / player->sprite->spDef.maxHealpoint;
			group1.shape.setSize({ newXH, 40 });
			group1.setString(str);
			window->draw(group1.shape);
			window->draw(group1.text);

			oss.str("");
			oss.clear();
			oss << std::fixed << std::setprecision(2) << player->timerBoost;
			oss << " / ";
			oss << std::fixed << std::setprecision(2) << player->timeBoost;
			str = oss.str();

			group2.shape.setFillColor({ 44, 148, 15 });
			float newXB = baseX * player->timerBoost / player->timeBoost;
			group2.shape.setSize({ newXB, 40 });
			group2.setString(str);
			window->draw(group2.shape);
			window->draw(group2.text);

			sf::CircleShape aim(player->getGun()->nowRad, 16);
			aim.setOrigin({ aim.getRadius(), aim.getRadius()});
			aim.setFillColor(sf::Color(0,0,0,0));
			aim.setOutlineColor(sf::Color::Black);
			aim.setOutlineThickness(1.5f);
			aim.setPosition({ SCREEN_W / 2, SCREEN_H / 2 });
			window->draw(aim);
		};
}

void UIManager::drawPlayerUI(Player* player)
{
	playerUI(player);
}