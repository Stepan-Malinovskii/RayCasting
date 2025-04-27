#include "UIManeger.h"
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>

UIManager::UIManager(sf::RenderWindow* _window) : window{ _window } {}

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

void UIManager::initDialog(std::map<int, std::wstring, std::greater<int>> variants,
	std::wstring npcName)
{
	background = sf::Sprite(Resources::dialogBackground);
	background.setScale({ SCREEN_W / Resources::dialogBackground.getSize().x,
		SCREEN_H / Resources::dialogBackground.getSize().y });

	sf::RectangleShape nameBase{ {DIALOG_W, TEXTSIZE + 10} };
	nameBase.setFillColor(sf::Color(100, 100, 100));
	sf::Text nameText(npcName, Resources::UIFont, TEXTSIZE);
	Group g(nameBase, nameText);
	g.setPosition({ SCREEN_W / 2, INTERVAL});
	buttons.push_back(g);

	sf::RectangleShape dataBase{ {DIALOG_W, DIALOG_H / 2.5f} };
	dataBase.setFillColor(sf::Color(100, 100, 100));
	sf::Text dataText(splitText(variants[-1], DIALOG_W, 40), Resources::UIFont, TEXTSIZE - 10);
	Group g1(dataBase, dataText);
	g1.setPosition({ SCREEN_W / 2, g.getPosition().y + g1.getSize().y / 2 + INTERVAL});
	buttons.push_back(g1);

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

void UIManager::initStartMenu()
{
	background = sf::Sprite(Resources::menuBackground);
	background.setScale({ SCREEN_W / Resources::menuBackground.getSize().x,
		SCREEN_H / Resources::menuBackground.getSize().y });
	
	sf::Text text(L"���������� ����", Resources::UIFont, 50.0f);
	sf::RectangleShape shape( {text.getLocalBounds().width + 20.0f, 60.0f});
	shape.setFillColor(sf::Color(100, 100, 100));
	Button button(Group(shape, text));
	button.setPosition({ SCREEN_W / 2.0f, 2.0f * SCREEN_H / 3.0f - 35.0f });
	button.setFunc([=]() { keyButton = 0;});
	auto& state = GameState::getInstance();
	if (!state.data.isFirstGame) buttons.push_back(button);

	button.setString(L"����� ����");
	button.setPosition({ SCREEN_W / 2.0f, 2.0f * SCREEN_H / 3.0f + 35.0f });
	button.setFunc([=]() { keyButton = 1;});
	buttons.push_back(button);

	button.setString(L"�����");
	button.setPosition({ SCREEN_W / 2.0f, 2.0f * SCREEN_H / 3.0f + 105.0f });
	button.setFunc([=]() { keyButton = 2;});
	buttons.push_back(button);
}

void UIManager::initGameMenu()
{
	background = sf::Sprite(Resources::inventoryBackground);
	background.setScale({ SCREEN_W / Resources::inventoryBackground.getSize().x,
		SCREEN_H / Resources::inventoryBackground.getSize().y });

	sf::Text text(L"����������", Resources::UIFont, 50.0f);
	sf::RectangleShape shape({ text.getLocalBounds().width + 20.0f, 60.0f });
	shape.setFillColor(sf::Color(100, 100, 100));
	Button button(Group(shape, text));
	button.setPosition({ SCREEN_W / 2.0f, 2.0f * SCREEN_H / 3.0f - 35.0f });
	button.setFunc([=]() { keyButton = 0; });
	buttons.push_back(button);

	button.setString(L"���������");
	button.setPosition({ SCREEN_W / 2.0f, 2.0f * SCREEN_H / 3.0f + 35.0f });
	button.setFunc([=]() { keyButton = 1;});
	buttons.push_back(button);

	button.setString(L"�����");
	button.setPosition({ SCREEN_W / 2.0f, 2.0f * SCREEN_H / 3.0f + 105.0f });
	button.setFunc([=]() { keyButton = 2;});
	buttons.push_back(button);
}

void UIManager::initSetting()
{
	background = sf::Sprite(Resources::inventoryBackground);
	background.setScale({ SCREEN_W / Resources::inventoryBackground.getSize().x,
		SCREEN_H / Resources::inventoryBackground.getSize().y });

	sf::Text text(L"�����", Resources::UIFont, 50.0f);
	sf::RectangleShape shape({ text.getLocalBounds().width + 20.0f, 60.0f });
	shape.setFillColor(sf::Color(100, 100, 100));
	Button button(Group(shape, text));
	button.setPosition({ SCREEN_W / 2.0f, 80.0f });
	button.setFunc([=]() { keyButton = 0; });
	buttons.push_back(button);

	text.setString(L"+");
	sf::RectangleShape funcShape({ 60.0f, 60.0f });
	funcShape.setFillColor(sf::Color(50, 50, 50));
	Button funcButton(Group(funcShape, text));
	
	auto& state = GameState::getInstance();

	text.setString(L"��������� ��������: " + std::to_wstring(state.data.effectVolume));
	shape = sf::RectangleShape({ text.getLocalBounds().width + 20.0f, 60.0f });
	shape.setFillColor(sf::Color(100, 100, 100));
	button = Button(Group(shape, text));
	button.setPosition({ SCREEN_W / 2.0f, 150.0f});
	buttons.push_back(button);
	funcButton.setPosition({ button.getPosition().x + button.getSize().x / 2 + 30.0f, button.getPosition().y });
	funcButton.setFunc([&]() {state.data.effectVolume++;keyButton = 1;});
	buttons.push_back(funcButton);
	funcButton.setPosition({ button.getPosition().x - button.getSize().x / 2 - 30.0f, button.getPosition().y });
	funcButton.setFunc([&]() {state.data.effectVolume--;keyButton = 1;});
	funcButton.setString(L"-");
	buttons.push_back(funcButton);

	button.setString(L"��������� ������: " + std::to_wstring(state.data.soundVolume));
	button.setPosition({ SCREEN_W / 2.0f, 220.0f});
	buttons.push_back(button);
	funcButton.setString(L"+");
	funcButton.setPosition({ button.getPosition().x + button.getSize().x / 2 + 30.0f, button.getPosition().y });
	funcButton.setFunc([&]() {state.data.soundVolume++;keyButton = 1;});
	buttons.push_back(funcButton);
	funcButton.setPosition({ button.getPosition().x - button.getSize().x / 2 - 30.0f, button.getPosition().y });
	funcButton.setFunc([&]() {state.data.soundVolume--; keyButton = 1;});
	funcButton.setString(L"-");
	buttons.push_back(funcButton);
	
	funcButton.setFunc(nullptr);
	shape = sf::RectangleShape({shape.getSize().x / 2, 30.0f});
	shape.setFillColor(sf::Color(100, 100, 100));
	text = sf::Text(L"", Resources::UIFont, 20.0f);
	button = Button(Group(shape, text));
	funcShape.setSize({ 30.0f, 30.0f });
	funcButton = Button(Group(funcShape, text));
	sf::Vector2f pos{ SCREEN_W / 2.0f - shape.getSize().x / 2 - 15.0f, 280.0f };

	std::vector<std::pair<std::wstring, std::wstring>> keys = {
		{L"A", L"Move left"},
		{L"D", L"Move right"},
		{L"W", L"Move forward"}, 
		{L"Scroll", L"To swap gun"}, 
		{L"E", L"To interact"},
		{L"F", L"To break door"},
		{L"Q", L"To open inventory"},
		{L"H", L"To healing"},
		{L"Space", L"To jump"},
		{L"Shift", L"To run"},
		{L"LMB", L"To fire"},
		{L"ESC", L"To open menu"},
		};

	for (int i = 0; i < keys.size(); i++)
	{
		button.setString(keys[i].second);
		funcButton.setString(keys[i].first);
		button.setPosition(pos);
		funcButton.setPosition({ pos.x - button.getSize().x / 2 - funcShape.getSize().x / 2, pos.y });
		buttons.push_back(button);
		buttons.push_back(funcButton);

		if (i % 2 == 0)
		{
			pos.x += shape.getSize().x + 15.0f + funcButton.getSize().x; 
		}
		else
		{
			pos.x -= shape.getSize().x + 15.0f + funcButton.getSize().x;
			pos.y += 40.0f;
		}
	}
}

void UIManager::initTrade(std::map<int, Itemble*> variants, Player* player)
{
	background = sf::Sprite(Resources::tradeBackground);
	background.setScale({ SCREEN_W / Resources::tradeBackground.getSize().x,
		SCREEN_H / Resources::tradeBackground.getSize().y });

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

		text.setString(std::to_wstring(pair.second->cost) + L" ���");
		maxCost = std::fmaxf(text.getLocalBounds().width, maxCost);
	}
	
	sf::Vector2f pos(SCREEN_W / 2, size + interval * 2);
	sf::Vector2f textPos((SCREEN_W - DIALOG_W) / 2 - ICON_SIZE / 2, size + interval * 2);

	sf::RectangleShape rect({ DIALOG_W, size });
	sf::RectangleShape texture({ ICON_SIZE, ICON_SIZE });
	sf::RectangleShape texture1({ ICON_SIZE, size });
	Button b5({texture, {}});
	Button b6({texture1, {}});
	b5.setTexture(&Resources::itembleIcon);
	b6.setFillColor(sf::Color(70,70,70));

	rect.setFillColor(sf::Color(50, 50, 50));
	Group g2(rect, text);

	for (auto var : variants)
	{
		if (var.second == nullptr) continue;

		std::wstring line = toMax(var.second->name, maxName, size - 25) + L" | " + 
			toMax(std::to_wstring(var.second->cost) + L" ���", maxCost, size - 25) + L" | " +
			toMax(var.second->disc, maxDisc, size - 25);

		g2.setString(line);
		g2.setPosition(pos);

		b6.setPosition(textPos);
		b5.setPosition(textPos);
		b5.setTextureRect({ {ICON_SIZE * var.second->id, 0}, {ICON_SIZE, ICON_SIZE} });

		buttons.push_back(Button(g2));
		buttons.back().setFunc([=]() { keyButton = var.first;});
		buttons.push_back(b6);
		buttons.push_back(b5);

		pos.y += interval + g2.shape.getSize().y;
		textPos.y = pos.y;
	}

	sf::RectangleShape rect2(rect);
	rect2.setSize({ DIALOG_W, size - 20 });
	Group g9(rect2, text);
	g9.setString(L"������: " + std::to_wstring(player->money) + L" | ��������: " + std::to_wstring(player->details));
	g9.setPosition({SCREEN_W / 2, g9.getSize().y / 2 + interval});
	buttons.push_back(g9);

	sf::Text text1(text);
	text1.setString(L"�\n�\n�\n�\n�\n�");
	text1.setCharacterSize(50);
	sf::RectangleShape rect1({ size + 10, 350 });
	rect1.setFillColor(sf::Color(70, 70, 70));
	Group g3(rect1, text1);
	g3.setPosition({ (SCREEN_W + DIALOG_W) / 2 + g3.getSize().x, g3.getSize().y / 2 + 5});

	buttons.push_back(Button(g3));
	buttons.back().setFunc([=]() { keyButton = -200; });

	g3.setString(L"�\n�\n�\n�\n�");
	g3.setPosition({ (SCREEN_W + DIALOG_W) / 2 + g3.getSize().x , SCREEN_H - g3.getSize().y/2 - 5});
	
	buttons.push_back(Button(g3));
	buttons.back().setFunc([=]() { keyButton = -100; });
}

void UIManager::initInvent(std::map<Itemble*, int> items, Itemble* choose, Player* player)
{
	background = sf::Sprite(Resources::inventoryBackground);
	background.setScale({ SCREEN_W / Resources::inventoryBackground.getSize().x,
		SCREEN_H / Resources::inventoryBackground.getSize().y });

	sf::RectangleShape baseShape({ DIALOG_W / 2 + 15, DIALOG_H + 10});
	baseShape.setFillColor(sf::Color(70, 70, 70));
	Group baseGroup(baseShape, {});
	baseGroup.setPosition({ INTERVAL + baseShape.getSize().x / 2 - 5,
							INTERVAL + baseShape.getSize().y / 2 - 5});
	buttons.push_back(baseGroup);
	
	sf::RectangleShape invShape({ DIALOG_W / 4, ICON_SIZE / 2  });
	invShape.setFillColor(sf::Color(50, 50, 50));
	sf::Text invText(L"", Resources::UIFont, ICON_SIZE / 2 - 20);
	Group invGroup(invShape, invText);
	Button invBut(invGroup);

	sf::Vector2f pos{ DIALOG_W / 8 + INTERVAL, INTERVAL + invGroup.getSize().y / 2 };
	int i = 0;

	for (auto pair : items)
	{
		invBut.setPosition(pos);
		invBut.setString(pair.first->name + L" | " + std::to_wstring(pair.second) + L" ��");
		invBut.setFunc([=]() { keyButton = pair.first->id;});

		buttons.push_back(invBut);

		if (i % 2 == 0)
		{
			pos.x = DIALOG_W / 8 + invGroup.getSize().x + INTERVAL + 5;
		}
		else
		{
			pos.y += invGroup.getSize().y + 5;
			pos.x = DIALOG_W / 8 + INTERVAL;
		}
		i++;
	}

	if (choose != nullptr)
	{
		sf::RectangleShape dataShape({ DIALOG_W / 4 + 15, DIALOG_H / 3 });
		dataShape.setFillColor(sf::Color(70, 70, 70));
		Group dataGroup(dataShape, {});
		dataGroup.setPosition({ baseGroup.getPosition().x, INTERVAL + dataGroup.getSize().y / 2 });
		dataGroup.move({ dataGroup.getSize().x / 2 + baseGroup.getSize().x / 2 + INTERVAL, 0 });
		buttons.push_back(dataGroup);

		sf::RectangleShape textureShape({ ICON_SIZE, ICON_SIZE });
		textureShape.setScale({ 2, 2 });
		Group textureGroup(textureShape, {});
		textureGroup.setPosition({ dataGroup.getPosition().x, INTERVAL + ICON_SIZE / 2 + 10 });
		buttons.push_back(Button(textureGroup));
		buttons.back().setTexture(&Resources::itembleIcon);
		buttons.back().setTextureRect({ {ICON_SIZE * choose->id, 0},{ICON_SIZE, ICON_SIZE} });

		Group dataGroup1(invGroup);
		dataGroup1.setPosition(textureGroup.getPosition());
		dataGroup1.move({ 0, textureGroup.getSize().y / 2 + dataGroup1.getSize().y + 5 });
		dataGroup1.setString(choose->name);
		buttons.push_back(dataGroup1);

		float oldSize = dataGroup1.getSize().y;
		dataGroup1.setSize({ dataGroup1.getSize().x, dataGroup1.getSize().y * 2 });
		dataGroup1.move({ 0, oldSize / 2 + dataGroup1.getSize().y / 2 + 5 });

		oldSize = dataGroup.getSize().y;
		dataGroup.setSize({ dataGroup.getSize().x, DIALOG_H * 2 / 3 - INTERVAL });
		dataGroup.move({ 0, oldSize / 2 + dataGroup.getSize().y / 2 + INTERVAL });
		buttons.push_back(Button(dataGroup));

		Group makeGroup(invGroup);
		sf::Vector2f pos = dataGroup.getPosition();
		pos.y -= dataGroup.getSize().y / 2 - 5 - makeGroup.getSize().y / 2;
		makeGroup.setPosition(pos);

		std::wostringstream oss;
		if (auto item = dynamic_cast<Item*>(choose); item)
		{
				makeGroup.setString(L"������������");
				buttons.push_back(Button(makeGroup));
				buttons.back().setFunc([&]() { keyButton = 100;});

			oss << choose->disc;
		}
		else if (auto gun = dynamic_cast<Gun*>(choose); gun)
		{
			if (player->guns[1] != nullptr)
			{
				makeGroup.setString(L"������ ������ " + player->guns[1]->name);
			}
			else
			{
				makeGroup.setString(L"������ �� ������ ����");
			}
			buttons.push_back(Button(makeGroup));
			buttons.back().setFunc([&]() { keyButton = 100;});
			makeGroup.move({ 0, makeGroup.getSize().y + 5 });

			if (player->guns[2] != nullptr)
			{
				makeGroup.setString(L"������ ������ " + player->guns[2]->name);
			}
			else
			{
				makeGroup.setString(L"������ �� ������ ����");
			}
			buttons.push_back(Button(makeGroup));
			buttons.back().setFunc([&]() { keyButton = 101;});

			int i = 102;
			for (auto it : gun->improvement)
			{
				makeGroup.move({ 0, makeGroup.getSize().y + 5 });
				makeGroup.setString(L"����� " + it.second->name);
				buttons.push_back(Button(makeGroup));
				buttons.back().setFunc([=]() { keyButton = i;});
				i++;
			}

			oss << L"����: " << std::fixed << std::setprecision(2) << gun->damage;
			oss << " | ";
			oss << L"������: " << std::fixed << std::setprecision(2) << gun->maxCount;
			oss << " | ";
			oss << L"�������: " << std::fixed << std::setprecision(2) << gun->maxImpRad;
		}
		else if (auto imp = dynamic_cast<Improve*>(choose); imp)
		{
			if (player->guns[1] != nullptr)
			{
				makeGroup.setString(L"������ �� " + player->guns[1]->name);
				buttons.push_back(Button(makeGroup));
				buttons.back().setFunc([&]() { keyButton = 100;});
				makeGroup.move({ 0, makeGroup.getSize().y + 5 });
			}

			if (player->guns[2] != nullptr)
			{
				makeGroup.setString(L"������ �� " + player->guns[2]->name);
				buttons.push_back(Button(makeGroup));
				buttons.back().setFunc([&]() { keyButton = 101;});
			}

			oss << choose->disc;
		}
		dataGroup1.setString(splitText(oss.str(), dataGroup1.getSize().x, dataGroup1.text.getCharacterSize()));
		buttons.push_back(dataGroup1);
	}
}

void UIManager::deleteNow() 
{ 
	buttons.clear();
	choseBut = nullptr;
}

int UIManager::checkButton()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::Vector2i worldPos = (sf::Vector2i)window->mapPixelToCoords(mousePos);
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].isClicked(worldPos))
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
	window->draw(background);
	for (auto b : buttons)
	{
		window->draw(b);
	}
}

void UIManager::drawPlayerUI(Player* player)
{
	sf::Text weaponInfo;

	if (!player->kick->isCanUsed())
	{
		player->kick->drawWeapon(window, { 0,0 });
	}
	else
	{
		auto gun = player->getNowGun();
		gun->drawWeapon(window, player->getDeltaShake());

		if (gun->isReset)
		{
			weaponInfo = sf::Text(std::to_string(player->patrons), Resources::UIFont, 30);
			auto b = weaponInfo.getLocalBounds();
			weaponInfo.setOrigin({ b.width / 2, b.height / 2 });
			weaponInfo.setPosition({ SCREEN_W / 2, SCREEN_H / 2 - weaponInfo.getCharacterSize() / 4 });
			weaponInfo.setPosition({ SCREEN_W - b.width / 2 - 20, SCREEN_H - 30 });
			weaponInfo.setFillColor({ 0, 0, 0 });
			window->draw(weaponInfo);

			weaponInfo.setString(std::to_string(gun->nowCount) + " / " + std::to_string(gun->maxCount));
			b = weaponInfo.getLocalBounds();
			weaponInfo.setOrigin({ b.width / 2, b.height / 2 });
			weaponInfo.setPosition({ SCREEN_W / 2, SCREEN_H / 2 - weaponInfo.getCharacterSize() / 4 });
			weaponInfo.setPosition({ SCREEN_W - b.width / 2 - 20, SCREEN_H - 60 });
			window->draw(weaponInfo);
		}
	}
	float baseX = 300;
	sf::RectangleShape baseShape({ baseX, 40 });
	baseShape.setFillColor({ 128, 128, 128 });
	sf::Text text("", Resources::UIFont, 30);
	Group group1(baseShape, text);
	group1.setPosition({ 170, SCREEN_H - 120 });
	window->draw(group1.shape);

	Group group2(group1);
	group2.setPosition({ group2.getPosition().x, group2.getPosition().y + 40 });
	window->draw(group2.shape);

	Group group3(group1);
	group3.setPosition({ group3.getPosition().x, group3.getPosition().y + 80 });
	window->draw(group3.shape);

	std::wostringstream oss;
	oss << std::fixed << std::setprecision(2) << player->enemy->spMap.nowHealPoint;
	oss << " / ";
	oss << std::fixed << std::setprecision(2) << player->enemy->enemyDef.maxHealpoint;
	std::wstring str = oss.str();

	group1.shape.setFillColor({ 255, 23, 23 });
	float newXH = baseX * (player->enemy->spMap.nowHealPoint <= 0 ? 0 :
		player->enemy->spMap.nowHealPoint) / player->enemy->enemyDef.maxHealpoint;
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

	if (player->nowHeal)
	{
		sf::RectangleShape rect({ ICON_SIZE, ICON_SIZE });
		rect.setTexture(&Resources::itembleIcon);
		rect.setTextureRect({ {player->nowHeal->id * ICON_SIZE, 0},{ICON_SIZE, ICON_SIZE} });
		auto b = rect.getLocalBounds();
		rect.setOrigin({ b.width / 2, b.height / 2 });
		if (weaponInfo.getString() != "")
		{
			rect.setPosition({ SCREEN_W - ICON_SIZE - weaponInfo.getLocalBounds().width,  SCREEN_H - ICON_SIZE / 2 });
		}
		else
		{
			rect.setPosition({ SCREEN_W - b.width / 2 - 20, SCREEN_H - ICON_SIZE / 2 });
		}
		window->draw(rect);
	}

	sf::CircleShape aim(player->getNowGun()->nowRad, 16);
	aim.setOrigin({ aim.getRadius(), aim.getRadius() });
	aim.setFillColor(sf::Color(0, 0, 0, 0));
	aim.setOutlineColor(sf::Color::Black);
	aim.setOutlineThickness(1.5f);
	aim.setPosition({ SCREEN_W / 2, SCREEN_H / 2 });
	window->draw(aim);
}