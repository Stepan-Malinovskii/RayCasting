#include "Game.h"

Game::Game(sf::RenderWindow* _window, Map* _nowMap) :
	window{ _window }, nowMap{ _nowMap }
{
	screenMidlePos = { (int)(SCREEN_W / 2), (int)(SCREEN_H / 2) };
	data = new Data();
	weaponManager = new WeaponManager(data);
	renderer = new Renderer(window);
	uiManager = new UIManager(window);
	dialogSys = new Dialog(window, data, uiManager, weaponManager);
	spManager = new SpriteManager(nowMap, data, dialogSys);
	initPlayer();

	dialogSys->setPlayer(player);
}

Game::~Game()
{
	delete data;
	delete dialogSys;
	delete spManager;
	delete weaponManager;
	delete uiManager;
}

void Game::initPlayer()
{
	player = spManager->getPlayer();
	player->kick = weaponManager->getGun(0);
	player->setGun(weaponManager->getGun(1));

	PlayerDef plDef = data->getPlayerData();

	for (auto it : plDef.itemData)
	{
		player->takeItem(weaponManager->getItem(it.first), it.second);
	}
}

void Game::save()
{
	spManager->saveSprite();
	weaponManager->saveGun();
	data->savePlayerData(player);
}

void Game::getInput(sf::Event event, float deltaTime)
{
	if (dialogSys->isActive) { return; }
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{			
		player->fire(0);
	}
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		bool flag = event.mouseWheelScroll.delta > 0 ? true : false;
		player->swapGun(flag);
	}

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::H)
		{
			player->heal();
		}
	}
}

void Game::getInput(float deltaTime)
{
	float radiansAngle = player->sprite->spMap.angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));
	sf::Vector2f horizontalMoveParametrs(-verticalMoveParametrs.y, verticalMoveParametrs.x);
	sf::Vector2f deltaPos(0, 0);
	bool lShiftPressed = false;
	float deltaX = 0, deltaY = 0;
	if (window->hasFocus())
	{
		if (GetAsyncKeyState('A'))
		{
			deltaPos -= horizontalMoveParametrs;
		}
		else if (GetAsyncKeyState('D'))
		{
			deltaPos += horizontalMoveParametrs;
		}
		if (GetAsyncKeyState('W'))
		{
			deltaPos += verticalMoveParametrs;
		}
		else if (GetAsyncKeyState('S'))
		{
			deltaPos -= verticalMoveParametrs;
		}
		if (GetAsyncKeyState('R'))
		{
			player->reloadingGun();
		}
		if (GetAsyncKeyState(VK_LSHIFT))
		{
			lShiftPressed = true;
		}
		if (GetAsyncKeyState(VK_SPACE))
		{
			player->jump();
		}
		if (GetAsyncKeyState('F'))
		{
			player->fire();
		}
		if (GetAsyncKeyState('E'))
		{
			Sprite* sp = player->dialog();
			if (sp != nullptr)
			{
				Npc* npc = dynamic_cast<Npc*>(sp);
				if (npc != nullptr)
				{
					npc->use();
				}
			}
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		deltaX = (mousePos.x - screenMidlePos.x) / 2.0f;
		deltaY = (mousePos.y - screenMidlePos.y) / 2.0f;
		sf::Mouse::setPosition(screenMidlePos, *window);
	}

	player->checkBoost(lShiftPressed, deltaTime);
	player->move(deltaPos, deltaTime);
	player->updateMouseData({deltaX, deltaY}, deltaTime);
}

void Game::resetMap(Map* newMap)
{
	nowMap = newMap;
	player = spManager->resetMap(newMap);
	player->kick = weaponManager->getGun(0);
	for (int i = 1; i < 8; i++)
	{
		player->setGun(weaponManager->getGun(i));
	}
}

void Game::update(float deltaTime)
{
	spManager->update(deltaTime);
}

void Game::makeCycle(float deltaTime)
{
#if !_DEBUG
	if (deltaTime > 1/50.0f)
	{
		deltaTime = 0;
	}
#endif //_DEBUG
	if (dialogSys->isActive)
	{
		window->clear();
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
	renderer->Draw3DView(player, nowMap, spManager->getSprites());
	uiManager->drawPlayerUI(player);
}
