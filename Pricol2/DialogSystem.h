#pragma once
#ifndef DIALOG
#define DIALOG

#include "sfmlExtantion.h"
#include <functional>
#include "Player.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Mouse.hpp"
#include "UIManeger.h"
#include "GunManager.h"

class Dialog
{
public:
	Dialog(sf::RenderWindow* _window, Data* _data, UIManager* _uiManager, 
		WeaponManager* _weaponManager);
	void setPlayer(Player* _player);
	void start(int key, std::wstring name = L"");
	void update();
	void draw();
	bool isActive;
private:
	bool isTrade;
	std::wstring name;
	Data* data;
	Player* player;
	int nowKey;
	int startKey;
	UIManager* uiManager;
	WeaponManager* weaponManager;
	sf::RenderWindow* window;

	Itemble* choose;
	std::map<int, Itemble*> title;

	void buy();
	void check();
	void checkTrade();
	void checkDialog();
	void init();
	void initTrade();
	void initDialog();
	void stop();
};

#endif // !DIALOG