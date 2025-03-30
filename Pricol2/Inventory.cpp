#include "Inventory.h"

Inventory::Inventory(Player* _player, UIManager* _uiManager):
	uiManager{ _uiManager}, player{ _player } {}

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
