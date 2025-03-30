#pragma once
#ifndef INVENT
#define INVENT

#include "UIManeger.h"
#include "GunManager.h"
#include "DataBase.h"

class Inventory
{
public:
	Inventory(sf::RenderWindow* _window, Player* _player, UIManager* _uiManager);

	Item* takeMaxHeal();
	void takeItem(Itemble*, int cnt = 1);
	void useItem(Itemble*, int cnt = 1);
	std::vector < std::pair<int, int>> getInventToSave();

	void useInvent();
	void update();
	void drawInvent();

	bool isOpen;
private:
	std::map<Itemble*, int> items;
	UIManager* uiManager;
	sf::RenderWindow* window;
	Player* player;
};

#endif // !INVENT