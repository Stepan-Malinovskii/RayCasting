#include "DialogSystem.h"

Dialog::Dialog(sf::RenderWindow* _window, Data* _data, UIManager* _uiManager,
	WeaponManager* _weaponManager) : window{ _window }, data{ _data },
	uiManager{ _uiManager }, weaponManager{ _weaponManager }
{
	nowKey = 0;
	startKey = 0;
	isActive = false;
	isTrade = false;
}

void Dialog::setPlayer(Player* _player) { player = _player; }

void Dialog::start(int key, std::wstring _name)
{
	window->setMouseCursorVisible(true);
	isActive = true;
	name = _name;
	nowKey = key;
	init();
}

void Dialog::stop()
{
	window->setMouseCursorVisible(false);
	isActive = false;
	isTrade = false;
	title.clear();
	uiManager->deleteNow();
}

void Dialog::buy()
{
	if (choose != nullptr)
	{
		if (choose->cost <= player->money)
		{
			player->money -= choose->cost;

			player->takeItem(choose);
			init();
		}
		choose = nullptr;
	}
}

void Dialog::check()
{
	if (isTrade) 
	{
		if (nowKey == -100)
		{
			stop();
			return;
		}
		else if (nowKey == -200)
		{
			buy();
		}
		else
		{
			choose = title[nowKey];
			return;
		}
	}
	else
	{
		if (nowKey == 0)
		{
			stop();
			return;
		}
		else
		{
			if (nowKey >= 400)
			{
				isTrade = true;
				startKey = nowKey;
				nowKey = 0;
				start(startKey);
				return;
			}
		}
	}
	init();
}

void Dialog::init()
{
	if (!isTrade)
	{
		auto keys = data->getKeys(nowKey);

		std::map<int, std::wstring, std::greater<int>> variants;
		for (int i = 0; i < keys.size(); i++)
		{
			auto d = data->getText(keys[i]);
			variants[d.second] = d.first;
		}

		uiManager->initDialog(variants, name);
	}
	else
	{
		if (startKey == 400)
		{
			auto item1 = weaponManager->getGuns();
			int i = 3;
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

		uiManager->initTrade(title, player);
	}
}

void Dialog::update()
{
	if (window->hasFocus())
	{
		static bool flag = false;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !flag)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			sf::Vector2i worldPos = (sf::Vector2i)window->mapPixelToCoords(mousePos);

			flag = true;
			int key = uiManager->checkButton(worldPos);
			if (key != -1) 
			{
				nowKey = key;
				check();
			}
		}
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			flag = false;
		}
	}
}

void Dialog::draw()
{
	uiManager->drawNow();
}