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
	void useMenu();
	void draw();
	void update();
private:
	bool isOpen;
	sf::RenderWindow* window;
	UIManager* uiManager;
	RenderState menuState;
};
#endif // !MENU