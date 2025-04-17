#include "DialogSystem.h"

Dialog::Dialog(sf::RenderWindow* _window, UIManager* _uiManager,
	WeaponManager* _weaponManager) : window{ _window },
	uiManager{ _uiManager }, weaponManager{ _weaponManager }
{
	nowKey = 0;
	startKey = 0;
	isActive = false;
	isTrade = false;
	choose = nullptr;
	player = nullptr;

	auto updateF = [=](float deltaTime) {
		update();
		};
	auto drawF = [=]() {
		draw();
		};
	dialogState = RenderState(updateF, drawF);
}

void Dialog::setPlayer(Player* _player) { player = _player; }

RenderState* Dialog::start(int key, std::wstring _name)
{
	window->setMouseCursorVisible(true);
	isActive = true;
	name = _name;
	nowKey = key;
	init();
	return &dialogState;
}

void Dialog::stop()
{
	window->setMouseCursorVisible(false);
	isActive = false;
	isTrade = false;
	title.clear();
	uiManager->deleteNow();
	onDialogEnd();
}

void Dialog::buy()
{
	if (!choose) return;
	
	if (choose->cost > player->money) return;
	
	player->money -= choose->cost;

	player->takeItem(choose);
	choose = nullptr;

	init();
}

void Dialog::check()
{
	if (isTrade)
	{
		checkTrade();
	}
	else
	{
		checkDialog();
	}
}

void Dialog::checkDialog()
{
	if (nowKey == 0)
	{
		stop();
	}
	else if (nowKey >= 400)
	{
		isTrade = true;
		startKey = nowKey;
		nowKey = 0;
		start(startKey);
	}
	else
	{
		init();
	}
}

void Dialog::checkTrade()
{
	if (nowKey == -100)
	{
		stop();
	}
	else if (nowKey == -200)
	{
		buy();
	}
	else
	{
		choose = title[nowKey];
	}
}

void Dialog::init()
{
	if (isTrade) 
	{
		initTrade();
	}
	else 
	{
		initDialog();
	}
}

void Dialog::initDialog()
{
	auto& data = Data::getInstance();
	auto keys = data.getKeys(nowKey);

	std::map<int, std::wstring, std::greater<int>> variants;

	for (int i = 0; i < keys.size(); i++)
	{
		auto d = data.getText(keys[i]);
		variants[d.second] = d.first;
	}

	uiManager->initDialog(variants, name);
}

void Dialog::initTrade()
{
	title.clear();

	if (startKey == 400)
	{
		int i = 3;

		auto item1 = weaponManager->getGuns();

		for (; i < item1.size(); i++)
		{
			title[i] = (Itemble*)item1[i];
		}

		auto item2 = weaponManager->getImprovs();

		for (; i < item2.size() + item1.size(); i++)
		{
			title[i] = (Itemble*)item2[i - item1.size()];
		}
	}
	else if (startKey == 401)
	{
		auto item = weaponManager->getItems();

		for (int i = 0; i < item.size(); i++)
		{
			title[i] = (Itemble*)item[i];
		}
	}
	else if (startKey == 402)
	{
		auto item = weaponManager->getTravelItem();

		for (int i = 0; i < item.size(); i++)
		{
			title[i] = item[i];
		}
	}

	uiManager->initTrade(title, player);
}

void Dialog::update()
{
	if (!window->hasFocus()) return;
	
	static bool isMouseDown = false;
	bool isPress = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	if (isPress && !isMouseDown)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		sf::Vector2i worldPos = (sf::Vector2i)window->mapPixelToCoords(mousePos);

		int key = uiManager->checkButton(worldPos);

		if (key != -1)
		{
			nowKey = key;
			check();
		}

		isMouseDown = true;
	}
	if (!isPress)
	{
		isMouseDown = false;
	}
}

void Dialog::draw()
{
	uiManager->drawNow();
}