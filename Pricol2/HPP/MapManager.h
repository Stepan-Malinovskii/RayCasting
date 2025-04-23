#pragma once
#ifndef MAPM
#define MAPM

#include "Map.h"
#include "Leaf.h"
#include "Resources.h"
#include "SoundManager.h"
#include "EventSystem.h"
#include <SFML/Graphics/RenderWindow.hpp>

constexpr int ENEMY_LEVEL_COUNT = 40;
constexpr int NEXT_LEVEL_N = -1, BASE_N = 0, ARENA_1_N = 1, ARENA_2_N = 2, ARENA_3_N = 3, BOSS_N = 4;

class MapManager
{
public:
	MapManager(sf::RenderWindow* _window);
	~MapManager();

	void save();
	void load(std::string fileName = "");

	std::pair<sf::Vector2f, sf::Vector2f> nextLocation(int index);
	void rewriteSprites(std::vector<std::shared_ptr<Sprite>>* sprs);
	sf::Vector2f getStartPosition();
	void drawMap(int layer);
	Map* getNowMap();
	bool isBase;
private:
	int mapNumber;
	Map* nowMap;
	sf::RenderWindow* window;
	sf::Vector2f startPos;
	sf::Vector2f endPos;

	void generate();
	void findStEnd(std::vector<Leaf*> leafs);
	void writeRoom(sf::IntRect rect, int layer, int value);
	void writeEnemy(std::vector<sf::IntRect> rooms);
};

static std::vector<std::string> mapFileNames{
	"base.map",
	"arena1.map",
	"arena2.map",
	"arena3.map",
	"boss.map" };

#endif // !MAPM