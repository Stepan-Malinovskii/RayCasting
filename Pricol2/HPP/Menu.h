#pragma once
#ifndef MENU
#define MENU

#include "RenderState.h"
#include "EventSystem.h"
#include "UIManeger.h"

class Menu
{
public:
	Menu(sf::RenderWindow* _window, UIManager* _uiManager);
	void start();
	void stop();
	void draw();
	void update();
private:
	sf::RenderWindow* window;
	UIManager* uiManager;
	RenderState menuState;
};
#endif // !MENU