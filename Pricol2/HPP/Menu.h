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
	void initStartMenu();
	void initGameMenu();
	void initSetting();
	void stop();
	void draw();
	void updateStartMenu();
	void updateGameMenu();
	void updateSetting();
private:
	bool isKeyPressed;
	sf::RenderWindow* window;
	UIManager* uiManager;
	RenderState startMenuState;
	RenderState gameMenuState;
	RenderState settingState;
};
#endif // !MENU