#pragma once
#ifndef DIALOG

#include "Sprite.h"
#include "DataBase.h"
#include "Trade.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

class Dialog
{
public:
	Dialog(sf::RenderWindow* _window, Data* data, UIManager* _uiManager, Trade* _trade);
	void start(Npc* npc);
	void update();
	void draw();
	bool isActive;
private:
	Npc* npc;
	Data* data;
	int nowKey;
	UIManager* uiManager;
	Trade* trade;
	sf::RenderWindow* window;

	void check();
	void init();
	void stop();
};
#endif // !DIALOG