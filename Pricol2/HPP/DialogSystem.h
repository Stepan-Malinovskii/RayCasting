#pragma once
#ifndef DIALOG
#define DIALOG

#include "sfmlExtantion.h"
#include <functional>
#include "Player.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Mouse.hpp"
#include "UIManeger.h"
#include "ItemManager.h"
#include "RenderState.h"
#include "EventSystem.h"

class Dialog
{
public:
	Dialog(sf::RenderWindow* _window, UIManager* _uiManager, 
		ItemManager* _weaponManager);
	void setPlayer(Player* _player);
	void start(Npc* npc);
private:
	RenderState dialogState;
	Npc* npc;
	Player* player;
	UIManager* uiManager;
	ItemManager* weaponManager;
	sf::RenderWindow* window;

	void update();
	void draw();
	void stop();
};

#endif // !DIALOG