#pragma once
#ifndef DIALOG
#define DIALOG

#include "sfmlExtantion.h"
#include <functional>
#include"Sprite.h"
#include "DataBase.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Resources.h"
#include "UIManeger.h"
#include "Trade.h"

class Dialog
{
public:
	Dialog(sf::RenderWindow* _window, Data* data, UIManager* _uiManager);
	void start(Npc* npc);
	void update();
	void draw();
	void setTrade(Trade* _trade);
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