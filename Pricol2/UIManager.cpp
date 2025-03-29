#include "UIManeger.h"
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>

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

void UIManager::initDialog(std::map<int, std::wstring, std::greater<int>> variants,
	std::wstring npcName)
{
	deleteNow();

	sf::RectangleShape nameBase{ {DIALOG_W, TEXTSIZE + 10} };
	nameBase.setFillColor(sf::Color(100, 100, 100));
	sf::Text nameText(npcName, Resources::UIFont, TEXTSIZE);
	Group g(nameBase, nameText);
	g.setPosition({ SCREEN_W / 2, INTERVAL});
	buttons.push_back(Button(g));

	sf::RectangleShape dataBase{ {DIALOG_W, DIALOG_H / 2.5f} };
	dataBase.setFillColor(sf::Color(100, 100, 100));
	sf::Text dataText(splitText(variants[-1], DIALOG_W, 40), Resources::UIFont, TEXTSIZE - 10);
	Group g1(dataBase, dataText);
	g1.setPosition({ SCREEN_W / 2, g.getPosition().y + g1.getSize().y / 2 + INTERVAL});
	buttons.push_back(Button(g1));

	sf::Vector2f pos(SCREEN_W / 2, g1.getPosition().y + g1.getSize().y / 2 + INTERVAL);

	
	for (auto var : variants)
	{
		if (var.first == -1) continue;
		sf::Text text(var.second, Resources::UIFont, TEXTSIZE);
		sf::RectangleShape rect({ DIALOG_W, TEXTSIZE + 10 });
		rect.setFillColor(sf::Color(50, 50, 50));
		Group g2(rect, text);
		g2.setPosition(pos);
		
		buttons.push_back(Button(g2));
		buttons.back().setFunc([=]() { keyButton = var.first;});

		pos.y +=  INTERVAL + g2.shape.getSize().y / 2;
	}
}

std::wstring UIManager::toMax(std::wstring str, float maxW, float textSize)
{
	sf::Text text(L"", Resources::UIFont, textSize);
	std::wstring result = str;
	while (true)
	{
		text.setString(result);

		if (text.getLocalBounds().width > maxW)
		{
			break;
		}

		result += L" ";
	}
	return result;
}

void UIManager::initTrade(std::map<int, Itemble*> variants, Player* player)
{
	deleteNow();

	float interval = 5.0f;
	float size = 44;

	float maxName = 0, maxDisc = 0, maxCost = 0;
	sf::Text text(L"", Resources::UIFont, size - 25);
	
	for (auto pair : variants)
	{
		if (pair.second == nullptr) continue;
		
		text.setString(pair.second->name);
		maxName = std::fmaxf(text.getLocalBounds().width, maxName);

		text.setString(pair.second->disc);
		maxDisc = std::fmaxf(text.getLocalBounds().width, maxDisc);

		text.setString(std::to_wstring(pair.second->cost) + L" Û·");
		maxCost = std::fmaxf(text.getLocalBounds().width, maxCost);
	}
	
	sf::Vector2f pos(SCREEN_W / 2, INTERVAL);

	sf::RectangleShape rect({ DIALOG_W, size });
	rect.setFillColor(sf::Color(50, 50, 50));
	Group g2(rect, text);

	for (auto var : variants)
	{
		if (var.second == nullptr) continue;

		std::wstring line = toMax(var.second->name, maxName, size - 25) + L" | " + 
			toMax(std::to_wstring(var.second->cost) + L" Û·", maxCost, size - 25) + L" | " +
			toMax(var.second->disc, maxDisc, size - 25);

		g2.setString(line);
		g2.setPosition(pos);

		buttons.push_back(Button(g2));
		buttons.back().setFunc([=]() { keyButton = var.first;});

		pos.y += interval + g2.shape.getSize().y;
	}

	sf::Text text1(text);
	text1.setString(L" \n”\nœ\n»\n“\n‹");
	text1.setCharacterSize(50);
	sf::RectangleShape rect1({ size, DIALOG_H });
	rect1.setFillColor(sf::Color(50, 50, 50));
	Group g3(rect1, text1);
	g3.setPosition({ (SCREEN_W - DIALOG_W) / 2 - g3.getSize().x, SCREEN_H / 2});

	buttons.push_back(Button(g3));
	buttons.back().setFunc([=]() { keyButton = -2; });

	g3.setString(L"¬\n€\n’\nŒ\nƒ");
	g3.setPosition({ (SCREEN_W + DIALOG_W) / 2 + g3.getSize().x , SCREEN_H / 2});
	
	buttons.push_back(Button(g3));
	buttons.back().setFunc([=]() { keyButton = -1; });
}

void UIManager::deleteNow() 
{ 
	buttons.clear();
	choseBut = nullptr;
}

int UIManager::checkButton(sf::Vector2i mousePos)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].isClicked(mousePos))
		{
			if (choseBut != nullptr) choseBut->setFillColor(sf::Color(50, 50, 50));
			choseBut = &buttons[i];
			choseBut->setFillColor(sf::Color(223, 154, 51));
			buttons[i].use();
			return keyButton;
		}
	}
	return -1;
}

void UIManager::drawNow()
{
	sf::Sprite back(Resources::dialogBackround);
	back.setScale({ SCREEN_W / Resources::dialogBackround.getSize().x,
		SCREEN_H / Resources::dialogBackround.getSize().y });
	window->draw(back);
	for (auto b : buttons)
	{
		window->draw(b);
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
					weaponInfo.setPosition({ SCREEN_W - b.width / 2 - 20, SCREEN_H - 100 });
					weaponInfo.setFillColor({ 0, 0, 0 });
					window->draw(weaponInfo);
					weaponInfo.setString(std::to_string(player->patrons));
					b = weaponInfo.getLocalBounds();
					weaponInfo.setOrigin({ b.width / 2, b.height / 2 });
					weaponInfo.setPosition({ SCREEN_W / 2, SCREEN_H / 2 - weaponInfo.getCharacterSize() / 4 });
					weaponInfo.setPosition({ SCREEN_W - b.width / 2 - 20, SCREEN_H - 45 });
					window->draw(weaponInfo);
				}
			}
			float baseX = 300;
			sf::RectangleShape baseShape({ baseX, 40 });
			baseShape.setFillColor({ 128, 128, 128 });
			sf::Text text("", Resources::UIFont, 30);
			Group group1({baseShape, text});
			group1.setPosition({ 170, SCREEN_H - 120 });
			window->draw(group1.shape);

			Group group2(group1);
			group2.setPosition({ group2.getPosition().x, group2.getPosition().y + 40 });
			window->draw(group2.shape);

			Group group3(group1);
			group3.setPosition({ group3.getPosition().x, group3.getPosition().y + 80});
			window->draw(group3.shape);

			std::wostringstream oss;
			oss << std::fixed << std::setprecision(2) << player->sprite->spMap.nowHealPoint;
			oss << " / ";
			oss << std::fixed << std::setprecision(2) << player->sprite->spDef.maxHealpoint;
			std::wstring str = oss.str();
			
			group1.shape.setFillColor({ 255, 23, 23 });
			float newXH = baseX * (player->sprite->spMap.nowHealPoint <= 0 ? 0 :
				player->sprite->spMap.nowHealPoint) / player->sprite->spDef.maxHealpoint;
			group1.shape.setSize({ newXH, 40 });
			group1.setString(str);
			window->draw(group1.shape);
			window->draw(group1.text);

			oss.str(L"");
			oss.clear();
			oss << std::fixed << std::setprecision(2) << player->nowStrenght;
			oss << " / ";
			oss << std::fixed << std::setprecision(2) << player->maxStrenght;
			str = oss.str();

			group2.shape.setFillColor({ 70, 130, 80 });
			float newXD = baseX * player->nowStrenght / player->maxStrenght;
			group2.shape.setSize({ newXD, 40 });
			group2.setString(str);
			window->draw(group2.shape);
			window->draw(group2.text);

			oss.str(L"");
			oss.clear();
			oss << std::fixed << std::setprecision(2) << player->nowEnergy;
			oss << " / ";
			oss << std::fixed << std::setprecision(2) << player->maxEnergy;
			str = oss.str();

			group3.shape.setFillColor({ 44, 148, 15 });
			float newXB = baseX * player->nowEnergy / player->maxEnergy;
			group3.shape.setSize({ newXB, 40 });
			group3.setString(str);
			window->draw(group3.shape);
			window->draw(group3.text);
			
			sf::RectangleShape rect;
			text.setFillColor(sf::Color::Black);
			Group g4(rect, text);
			g4.setString(L"Money: " + std::to_wstring(player->money));
			g4.setPosition({ SCREEN_W - g4.text.getLocalBounds().width / 2 - 20, 50});
			window->draw(g4.text);

			g4.setString(L"Details: " + std::to_wstring(player->details));
			g4.setPosition({ SCREEN_W - g4.text.getLocalBounds().width / 2 - 20, 50 + INTERVAL });
			window->draw(g4.text);

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