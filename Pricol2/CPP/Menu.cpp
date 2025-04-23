#include "Menu.h"

Menu::Menu(sf::RenderWindow* _window, UIManager* _uiManager) : uiManager{_uiManager}, window{_window},
menuState{ [=](float deltaTime) {update();}, [=]() {draw();} } {}

void Menu::start()
{
	auto& event = EventSystem::getInstance();
	window->setMouseCursorVisible(true);
	uiManager->initMenu();
	event.trigger<RenderState*>("SWAPSTATE", &menuState);
	SoundManager::playerMusic(MenuSound);
}

void Menu::stop()
{
	auto& event = EventSystem::getInstance();
	window->setMouseCursorVisible(false);
	uiManager->deleteNow();
	event.trigger<RenderState*>("SWAPSTATE", nullptr);
}

void Menu::draw() { uiManager->drawNow(); }

void Menu::update()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) return;

	int key = uiManager->checkButton();

	if (key == -1) return;

	if (key == 0) {}
	else if (key == 1)
	{
		auto& event = EventSystem::getInstance();
		event.trigger<int>("RESET_GAME", 0);
	}
	else if (key == 2)
	{
		window->close();
	}

	stop();
}
