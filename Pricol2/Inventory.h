#pragma once
#ifndef INVENT
#define INVENT

#include "UIManeger.h"
#include "GunManager.h"
#include "DataBase.h"

class Inventory
{
public:
	Inventory(Player* _player, UIManager* _uiManager);

	Item* takeMaxHeal();
	void takeItem(Itemble*, int cnt = 1);
	void useItem(Itemble*, int cnt = 1);
	//void setInventFromLoad(std::vector < std::pair<int, int>> data);
	std::vector < std::pair<int, int>> getInventToSave();
private:

	std::map<Itemble*, int> items;
	UIManager* uiManager;
	Player* player;
};

#endif // !INVENT