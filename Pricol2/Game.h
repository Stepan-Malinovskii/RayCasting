#pragma once
#ifndef GAME
#define GAME

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <memory>
#include <vector>
#include <Windows.h>
#include "Sprite.h"
#include "DataBase.h"
#include "DialogSystem.h"
#include "Player.h"
#include "Renderer.h"
#include "SpriteManager.h"
#include "GunManager.h"
#include "UIManeger.h"
#include "Inventory.h"
#include "MapManager.h"
#include "SoundManager.h"
#include "RenderState.h"


class Game
{
public:
public:
	Game(sf::RenderWindow* window, MapManager* mapManager);
	~Game();
	void getInput(sf::Event event, float deltaTime);
	void makeCycle(float deltaTime);
	void save();
	void editor(); //Удалить после конца
private:
	void getInput(float deltaTime);
	void update(float deltaT);
	void render();
	void initPlayer();
	void swapLocation();

	RenderState* currentState;
	RenderState playState;
	Data* data;
	Inventory* invent;
	Dialog* dialogSys;
	sf::Vector2i screenMidlePos;
	SpriteManager* spManager;
	UIManager* uiManager;
	WeaponManager* weaponManager;
	Player* player;
	Renderer* renderer;
	MapManager* mapManager;
	sf::RenderWindow* window;
};

#endif // !GAME
