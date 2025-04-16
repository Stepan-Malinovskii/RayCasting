#include "Game.h"

Game::Game(sf::RenderWindow* _window, MapManager* _mapManager) :
	window{ _window }, mapManager{ _mapManager }
{
	screenMidlePos = { (int)(SCREEN_W / 2), (int)(SCREEN_H / 2) };
	weaponManager = new WeaponManager();
	renderer = new Renderer(window);
	uiManager = new UIManager(window);
	dialogSys = new Dialog(window, uiManager, weaponManager);
	spManager = new SpriteManager(mapManager->getNowMap(), dialogSys);
	player = spManager->getPlayer();
	invent = new Inventory(window, player, uiManager);
	initPlayer();

	auto& data = Data::getInstance();

	for (auto b : data.getInvent())
	{
		player->takeItem(weaponManager->getItem(b.first), b.second);
	}

	auto update = [=](float deltaTime) {
		getInput(deltaTime);
		spManager->update(deltaTime);
		};
	auto draw = [=]() {
		renderer->Draw3DView(player, mapManager->getNowMap(), spManager->getDeteachSprite());
		uiManager->drawPlayerUI(player);
		};
	playState = RenderState(update, draw);
	currentState = &playState;

	dialogSys->onDialogEnd = [=]() { currentState = &playState; };

	auto& event = EventSystem::getInstance();
	event.subscribe<int>("SWAPLOC", [=](const int levelN)
		{
			auto pair = mapManager->nextLocation();
			spManager->resetMap(mapManager->getNowMap(), pair);
		}
	);

	player->enemy->spMap.nowHealPoint = 180.0f; // ÂÛÐÅÇÀÒÜ ÏÎÒÎÌ
}

Game::~Game()
{
	delete dialogSys;
	delete spManager;
	delete weaponManager;
	delete uiManager;
	delete invent;
}

void Game::initPlayer()
{
	player->setInventory(invent);
	player->kick = weaponManager->getGunByIndex(0);
	player->setGun(weaponManager->getGunByIndex(1), 0);
	player->nowHeal = invent->takeMaxHeal();
	auto& data = Data::getInstance();
	PlayerDef plDef = data.getPlayerData();

	int i = 1;
	for (auto it : plDef.gunsData)
	{
		player->setGun(weaponManager->getGunById(it), i++);
	}

	dialogSys->setPlayer(player);
}

void Game::editor()
{
	spManager->resetMap(mapManager->getNowMap(), { {2.0f, 2.0f}, {} });
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
			currentState = invent->useInvent();
			if (!currentState) { currentState = &playState; }
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{			
		player->fire(0);
	}
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		player->swapGun(event.mouseWheelScroll.delta > 0);
	}

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::H)
		{
			player->heal();
		}

		if (event.key.code == sf::Keyboard::P)
		{
			auto& event = EventSystem::getInstance();
			event.trigger("SWAPLOC", -1);
		}
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
		Sprite* sp = player->dialog();
		if (sp != nullptr)
		{
			auto npc = spManager->getNpc(sp->id);
			if (npc)
			{
				currentState = dialogSys->start(npc->npcDefData.startKey, npc->spDef.name);
			}
		}
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
#ifdef NDEBUG
	deltaTime = std::min(deltaTime, 1.0f / 144.0f);
#endif

	window->clear();

	currentState->update(deltaTime);
	currentState->draw();
	SoundManager::update();
}
