#include "Game.h"

Game::Game(sf::RenderWindow* _window, MapManager* _mapManager) :
	window{ _window }, mapManager{ _mapManager }
{
	screenMidlePos = { (int)(SCREEN_W / 2), (int)(SCREEN_H / 2) };
	data = new Data();
	weaponManager = new WeaponManager(data);
	renderer = new Renderer(window);
	uiManager = new UIManager(window);
	dialogSys = new Dialog(window, data, uiManager, weaponManager);
	spManager = new SpriteManager(mapManager->getNowMap(), data, dialogSys);
	initPlayer();

	invent = new Inventory(window, player, uiManager);
	player->setInventory(invent);
	auto a = data->getInvent();

	for (auto b : a)
	{
		player->takeItem(weaponManager->getItem(b.first), b.second);
	}

	dialogSys->setPlayer(player);
}

Game::~Game()
{
	delete data;
	delete dialogSys;
	delete spManager;
	delete weaponManager;
	delete uiManager;
	delete invent;
}

void Game::editor()
{
	spManager->resetMap(mapManager->getNowMap(), { {2.0f, 2.0f},{}});
	initPlayer();
}

void Game::initPlayer()
{
	player = spManager->getPlayer();
	player->kick = weaponManager->getGunByIndex(0);
	player->setGun(weaponManager->getGunByIndex(1), 0);

	PlayerDef plDef = data->getPlayerData();
	player->patrons = 100;

	int i = 1;
	for (auto it : plDef.gunsData)
	{
		player->setGun(weaponManager->getGunById(it), i++);
	}
}

void Game::save()
{
	mapManager->rewriteSprites(spManager->getDeteachSprite());
	weaponManager->saveGun();
	data->savePlayerData(player);
	data->saveInvent(invent->getInventToSave());
	mapManager->save();
}

void Game::getInput(sf::Event event, float deltaTime)
{
	if (dialogSys->isActive) return;

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Q)
		{
			invent->useInvent();
		}
	}

	if (invent->isOpen) { return; }

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
			generate();
		}
	}
}

void Game::getInput(float deltaTime)
{
	if (!window->hasFocus()) return;

	float radAng = player->sprite->spMap.angle * PI / 180.0f;
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
			if (npc) npc->use();
		}
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	
	player->checkBoost(lShiftPressed, deltaTime);
	player->move(deltaPos, deltaTime);
	player->updateMouseData({ (mousePos.x - screenMidlePos.x) / 2.0f, 
							  (mousePos.y - screenMidlePos.y) / 2.0f }, deltaTime);
	sf::Mouse::setPosition(screenMidlePos, *window);
}

void Game::generate()
{
	data->savePlayerData(player);
	auto pair = mapManager->generate();
	spManager->resetMap(mapManager->getNowMap(), pair);
}

void Game::update(float deltaTime)
{
	spManager->update(deltaTime);
	SoundManager::update();
}

void Game::makeCycle(float deltaTime)
{
#ifdef NDEBUG
	deltaTime = std::min(deltaTime, 1.0f / 144.0f);
#endif

	window->clear();

	if (invent->isOpen)
	{
		invent->update();
		invent->drawInvent();
	}
	else if (dialogSys->isActive)
	{
		dialogSys->update();
		dialogSys->draw();
	}
	else
	{
		getInput(deltaTime);
		update(deltaTime);
		render();
	}
}

void Game::render()
{
	renderer->Draw3DView(player, mapManager->getNowMap(), spManager->getDeteachSprite());
	uiManager->drawPlayerUI(player);
}
