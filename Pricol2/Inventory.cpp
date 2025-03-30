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

void Inventory::useInvent() { isOpen = !isOpen; }

void Inventory::update()
{

}

void Inventory::drawInvent()
{

}
