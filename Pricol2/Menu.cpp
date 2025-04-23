#include "Menu.h"

Menu::Menu(sf::RenderWindow* _window, UIManager* _uiManager) : uiManager{_uiManager}, window{_window},
menuState{ [=](float deltaTime) {update();}, [=]() {draw();} }, isOpen{false} {}

void Menu::useMenu()
{
	auto& event = EventSystem::getInstance();
	if (isOpen)
	{
		window->setMouseCursorVisible(false);
		uiManager->deleteNow();
		isOpen = false;
		event.trigger<RenderState*>("SWAPSTATE", nullptr);
	}
	else
	{
		window->setMouseCursorVisible(true);
		uiManager->initMenu();
		isOpen = true;
		event.trigger<RenderState*>("SWAPSTATE", &menuState);
	}
}

void Menu::draw() { uiManager->drawNow(); }

void Menu::update()
{
	int key = uiManager->checkButton();
	if (key == -1) return;

	if (key == 0)
	{

	}
	else if (key == 1)
	{

	}
	else if (key == 2)
	{

	}
}
