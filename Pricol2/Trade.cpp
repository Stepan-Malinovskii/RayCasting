#include "Trade.h"
#include <SFML/Window.hpp>

Trade::Trade(UIManager* _uiManager, WeaponManager* _weaponManager,
	Player* _player, sf::RenderWindow* _window)
{
	uiManager = _uiManager;
	weaponManager = _weaponManager;
	player = _player;
	window = _window;
	nowKey = 0;
	startKey = 0;
	isActive = false;
}

void Trade::start(int key)
{
	isActive = true;
	startKey = key;
	init();
}

void Trade::stop()
{
	uiManager->deleteNow();
	isActive = false;
	title.clear();
	startKey = 0;
	nowKey = 0;
}

void Trade::check()
{
	if (nowKey == -1)
	{
		stop();
		return;
	}
	else if (nowKey == -2)
	{
		buy();
	}
	else
	{
		choose = title[nowKey];
	}
}

void Trade::init()
{
	title.clear();

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

void Trade::buy()
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

void Trade::update()
{
	if (window->hasFocus())
	{
		static bool flag = false;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !flag)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			sf::Vector2i worldPos = (sf::Vector2i)window->mapPixelToCoords(mousePos);

			flag = true;
			nowKey = uiManager->checkButton(worldPos);
			check();
		}
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			flag = false;
		}
	}
}