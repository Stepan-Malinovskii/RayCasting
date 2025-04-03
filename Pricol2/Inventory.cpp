#include "Inventory.h"

Inventory::Inventory(sf::RenderWindow* _window, Player* _player, UIManager* _uiManager) :
	window{ _window }, uiManager { _uiManager }, player{ _player } 
{
	isOpen = false;
}

Item* Inventory::takeMaxHeal()
{
	Item* nowHeal = nullptr;
	for (auto var : items)
	{
		auto t = dynamic_cast<Item*>(var.first);
		if (t != nullptr)
		{
			if (t->type == Heal)
			{
				if (nowHeal == nullptr || t->id > nowHeal->id)
				{
					nowHeal = t;
				}
			}
		}
	}
	return nowHeal;
}

void Inventory::takeItem(Itemble* item, int cnt)
{
	items[item] += cnt;
}

void Inventory::useItem(Itemble* item, int cnt)
{
	if (items[item] - cnt >= 0)
	{
		items[item] -= cnt;

		if (items[item] == 0)
		{
			items.erase(item);
		}
	}
}

std::vector<std::pair<int, int>> Inventory::getInventToSave()
{
	std::vector<std::pair<int, int>> inv;

	for (auto it : items)
	{
		inv.push_back({ it.first->id, it.second });
	}

	return inv;
}

void Inventory::useInvent() 
{ 
	isOpen = !isOpen;
	window->setMouseCursorVisible(isOpen);
	if (!isOpen)
	{
		uiManager->deleteNow();
		choose = nullptr;
	}
	else
	{
		initInv();
	}
}

void Inventory::initInv()
{
	uiManager->initInvent(items, choose);
}

void Inventory::checkChose()
{
	if (nowKey >= 100)
	{
		if (dynamic_cast<Item*>(choose) != nullptr)
		{
			auto item = dynamic_cast<Item*>(choose);
			item->useFunc(player);
			useItem(choose);
		}
		else if (dynamic_cast<Gun*>(choose) != nullptr)
		{
			auto gun = dynamic_cast<Gun*>(choose);
			if (nowKey == 100)
			{
				if (player->getGun(2) != gun)
				{
					player->setGun(gun, 1);
				}
			}
			else if (nowKey == 101)
			{
				if (player->getGun(1) != gun)
				{
					player->setGun(gun, 2);
				}
			}
			else
			{
				int i = 102;
				for (auto it : gun->improvement)
				{
					if (i == nowKey)
					{
						items[dynamic_cast<Itemble*>(gun->deleteImprove(it.first))] += 1;
						break;
					}
					i++;
				}
			}
		}
		else
		{
			auto imp = dynamic_cast<Improve*>(choose);
			if (nowKey == 100)
			{
				auto gun = player->getGun(1);
				if (gun != nullptr)
				{
					auto temp = gun->trySetImprove(imp);
					useItem(choose);
					if (temp != nullptr)
					{
						items[dynamic_cast<Itemble*>(temp)] += 1;
					}
				}
			}
			else if (nowKey == 101)
			{
				auto gun = player->getGun(2);
				if (gun != nullptr)
				{
					auto temp = gun->trySetImprove(imp);
					useItem(choose);
					if (temp != nullptr)
					{
						items[dynamic_cast<Itemble*>(temp)] += 1;
					}
				}
			}
		}
		choose = nullptr;
		initInv();
	}
	else
	{
		for (auto pair : items)
		{
			if (pair.first->id == nowKey)
			{
				choose = pair.first;
				initInv();
				break;
			}
		}
	}
}

void Inventory::update()
{
	if (!window->hasFocus()) return;
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
			checkChose();
		}
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		flag = false;
	}
}

void Inventory::drawInvent()
{
	uiManager->drawNow();
}
