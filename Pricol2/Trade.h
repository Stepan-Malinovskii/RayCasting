#pragma once
#ifndef TRADE
#define TRADE

#include "UIManeger.h"
#include "GunManager.h"

class Trade
{
public:
	Trade(UIManager* _uiManager, WeaponManager* _weaponManager,
		Player* _player, sf::RenderWindow* _window);
	void update();
	void start(int key);
	bool isActive;
private:
	int startKey;
	int nowKey;
	Itemble* choose;
	UIManager* uiManager;
	WeaponManager* weaponManager;
	Player* player;
	sf::RenderWindow* window;
	std::map<int, Itemble*> title;
	void stop();
	void buy();
	void check();
	void init();
};

#endif // !TRADE
