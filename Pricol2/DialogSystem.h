#pragma once

#include "sfmlExtantion.h"
#include <functional>
#include"Sprite.h"
#include "DataBase.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Resources.h"
#include "UIManeger.h"

class Dialog
{
public:
	Dialog(sf::RenderWindow* _window, Data* data);
	void init();
	void start(Npc* npc);
	void stop();
	void update();
	void draw();
	void check();
	bool isActive;
private:
	Npc* npc;
	Data* data;
	int nowKey;
	UIManager* uiManager;
	sf::RenderWindow* window;
};