#include "Game.h"

Game::Game(sf::RenderWindow* _window, MapManager* _mapManager) :
	window{ _window }, mapManager{ _mapManager }, isKeyPressed{false}
{
	screenMidlePos = { (int)(SCREEN_W / 2), (int)(SCREEN_H / 2) };
	itemManager = new ItemManager();
	renderer = new Renderer(window);
	uiManager = new UIManager(window);
	menu = new Menu(window, uiManager);
	dialogSys = new Dialog(window, uiManager, itemManager);
	spManager = new SpriteManager(mapManager->getNowMap(), uiManager, itemManager);
	player = spManager->getPlayer();
	invent = new Inventory(window, player, uiManager);
	initPlayer();

	auto& data = Data::getInstance();
	for (auto b : data.getInvent()) { player->takeItem(itemManager->getItemble(b.first), b.second); }

	auto update = [=](float deltaTime) {
		getInput(deltaTime);
		spManager->update(deltaTime);
		};
	auto draw = [=]() {
		renderer->Draw3DView(player, mapManager->getNowMap(), spManager->getDeteachSprite());
		uiManager->drawPlayerUI(player);
		};
	playState = RenderState(update, draw);

	auto& event = EventSystem::getInstance();
	event.subscribe<int>("SWAPLOC", [=](const int levelN)
		{
			auto& state = GameState::getInstance();
			state.data.changerCoef = Random::intRandom(2, 5);
			sf::Vector2f pos = mapManager->nextLocation(levelN);
			spManager->resetMap(mapManager->getNowMap(), pos);
		}
	);

	event.subscribe<int>("RESET_GAME", [=](const int NON) { currentState = &playState;
	player->guns[1] = nullptr; player->guns[2] = nullptr; invent->takeItem(itemManager->getGunByIndex(2), 1);
	player->setGun(itemManager->getGunByIndex(2), 1);});

	event.subscribe<RenderState*>("SWAP_STATE", [=](RenderState* state) {
		if (state)
		{
			currentState = state;
		}
		else
		{
			currentState = &playState;
			auto& state = GameState::getInstance();
			SoundManager::stopAllSound();
			if (state.data.isLevelBase) { SoundManager::playerMusic(BaseSound); }
			else { SoundManager::playerMusic(LevelSound); }
		}});

	menu->initStartMenu();

	//player->enemy->spMap.nowHealPoint = 100.0f; // �������� �����
	//player->money = 1000; // �������� �����
}

Game::~Game()
{
	delete dialogSys;
	delete spManager;
	delete itemManager;
	delete uiManager;
	delete invent;
	delete menu;
}

void Game::initPlayer()
{
	player->setInventory(invent);
	player->kick = itemManager->getGunByIndex(0);
	player->setGun(itemManager->getGunByIndex(1), 0);
	player->nowHeal = invent->takeMaxHeal();
	auto& data = Data::getInstance();
	PlayerDef plDef = data.getPlayerData();

	int i = 1;
	for (auto it : plDef.gunsData)
	{
		player->setGun(itemManager->getGunById(it), i++);
	}

	dialogSys->setPlayer(player);
}

void Game::editor()
{
	spManager->resetMap(mapManager->getNowMap(), {0.0f, 0.0f});
}

void Game::save()
{
	mapManager->rewriteSprites(spManager->getDeteachSprite());
	auto& event = EventSystem::getInstance();
	event.trigger<int>("SAVE", 0);
}

void Game::getInput(sf::Event event, float deltaTime)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Q)
		{
			invent->useInvent();
		}

		if (event.key.code == sf::Keyboard::H)
		{
			player->heal();
		}

		if (event.key.code == sf::Keyboard::P)
		{
			auto& event = EventSystem::getInstance();
			event.trigger<int>("SWAPLOC", BASE_N);
		}

		if (event.key.code == sf::Keyboard::Escape)
		{
			menu->initGameMenu();
		}
	}

	if (event.type == sf::Event::MouseWheelScrolled)
	{
		player->swapGun(event.mouseWheelScroll.delta > 0);
	}
}

void Game::getInput(float deltaTime)
{
	if (!window->hasFocus()) return;

	float radAng = player->enemy->spMap.angle * PI / 180.0f;
	sf::Vector2f vertParams(cos(radAng), sin(radAng));
	sf::Vector2f horParams(-vertParams.y, vertParams.x);
	sf::Vector2f deltaPos(0, 0);
	bool lShiftPressed = false;

	if (GetAsyncKeyState('A')) { deltaPos -= horParams; }
	else if (GetAsyncKeyState('D')) { deltaPos += horParams; }
	if (GetAsyncKeyState('W')) { deltaPos += vertParams; }
	else if (GetAsyncKeyState('S')) { deltaPos -= vertParams; }
	if (GetAsyncKeyState('R')) { player->reloadingGun(); }
	if (GetAsyncKeyState(VK_LSHIFT)) { lShiftPressed = true; }
	if (GetAsyncKeyState(VK_SPACE)) { player->jump(); }
	if (GetAsyncKeyState('F')) { player->fire(); }
	if (GetAsyncKeyState('E'))
	{
		if (Sprite* sp = player->dialog(); sp)
		{
			if (auto npc = spManager->getNpc(sp->id); npc)
			{
				dialogSys->start(npc);
			}
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		player->fire(0);
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	
	player->checkBoost(lShiftPressed, deltaTime);
	player->move(deltaPos, deltaTime);
	player->updateMouseData({ (mousePos.x - screenMidlePos.x) / 2.0f, 
							  (mousePos.y - screenMidlePos.y) / 2.0f }, deltaTime);
	sf::Mouse::setPosition(screenMidlePos, *window);
}

void Game::makeCycle(float deltaTime)
{
	currentState->update(deltaTime);
	currentState->draw();
	SoundManager::update();
}
