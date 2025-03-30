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
#include "Map.h"
#include "Player.h"
#include "Renderer.h"
#include "SpriteManager.h"
#include "GunManager.h"
#include "UIManeger.h"

class Game
{
public:
public:
	Game(sf::RenderWindow* window, Map* nowMap);
	~Game();
	void getInput(sf::Event event, float deltaTime);
	void resetMap(Map* newMap);
	void makeCycle(float deltaTime);
	void save();
private:
	void getInput(float deltaTime);
	void update(float deltaT);
	void render();
	void initPlayer();

	Data* data;
	Npc nowNpc;
	Dialog* dialogSys;
	sf::Vector2i screenMidlePos;
	SpriteManager* spManager;
	UIManager* uiManager;
	WeaponManager* weaponManager;
	Player* player;
	Renderer* renderer;
	Map* nowMap;
	sf::RenderWindow* window;
};

#endif // !GAME
