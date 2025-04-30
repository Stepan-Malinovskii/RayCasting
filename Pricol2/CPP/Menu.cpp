#include "Menu.h"

Menu::Menu(sf::RenderWindow* _window, UIManager* _uiManager, Player* _player) : 
	uiManager{_uiManager}, window{_window}, isKeyPressed{false}, player{_player},
	startMenuState{[=](float deltaTime) {updateStartMenu();}, [=]() {draw();}},
	gameMenuState{[=](float deltaTime) {updateGameMenu();},[=]() {draw();} },
	settingState{ [=](float deltaTime) {updateSetting();}, [=]() {draw();} } {}

void Menu::initStartMenu()
{
	auto& event = EventSystem::getInstance();
	window->setMouseCursorVisible(true);
	uiManager->initStartMenu();
	event.trigger<RenderState*>("SWAP_STATE", &startMenuState);
	SoundManager::playerMusic(MenuSound);
}

void Menu::initGameMenu()
{
	auto& event = EventSystem::getInstance();
	window->setMouseCursorVisible(true);
	uiManager->initGameMenu();
	event.trigger<RenderState*>("SWAP_STATE", &gameMenuState);
}

void Menu::initSetting()
{
	auto& event = EventSystem::getInstance();
	window->setMouseCursorVisible(true);
	uiManager->initSetting();
	event.trigger<RenderState*>("SWAP_STATE", &settingState);
}

void Menu::stop()
{
	auto& event = EventSystem::getInstance();
	window->setMouseCursorVisible(false);
	uiManager->deleteNow();
	event.trigger<RenderState*>("SWAP_STATE", nullptr);
	SoundManager::playerMusic(BaseSound);
}

void Menu::draw() { uiManager->drawNow(); }

void Menu::updateStartMenu()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		isKeyPressed = false;
		return;
	}

	if (isKeyPressed) return;

	isKeyPressed = true;

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

void Menu::updateGameMenu()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		isKeyPressed = false;
		return;
	}

	if (isKeyPressed) return;

	isKeyPressed = true;

	int key = uiManager->checkButton();

	if (key == -1) return;

	if (key == 0)
	{
		stop();
	}
	else if (key == 1)
	{
		uiManager->deleteNow();
		initSetting();
	}
	else if (key == 2)
	{
		auto& event = EventSystem::getInstance();
		event.trigger<int>("SAVE", 0);
		uiManager->deleteNow();
		initStartMenu();
	}
}

void Menu::updateSetting()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		isKeyPressed = false;
		return;
	}

	if (isKeyPressed) return;

	isKeyPressed = true;

	int key = uiManager->checkButton();

	if (key == -1) return;

	if (key == 0) 
	{
		uiManager->deleteNow();
		initGameMenu();
	}
	else if (key == 1)
	{
		SoundManager::updateVolume();
		uiManager->deleteNow();
		uiManager->initSetting();
	}
	else if (key == 2)
	{
		auto& state = GameState::getInstance();
		player->mouseSpeed = state.data.mouseSpeed;
		uiManager->deleteNow();
		uiManager->initSetting();
	}
}
