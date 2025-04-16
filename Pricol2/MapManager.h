#pragma once
#ifndef MAPM
#define MAPM

#include "Map.h"
#include "Leaf.h"
#include "Resources.h"
#include "SoundManager.h"
#include <SFML/Graphics/RenderWindow.hpp>

constexpr int ENEMY_LEVEL_COUNT = 40;
constexpr int BASE_N = 0, ARENA_1_N = 1, ARENA_2_N = 2, ARENA_3_N = 3, BOSS_N = 4;

class MapManager
{
public:
	MapManager(sf::RenderWindow* _window);
	~MapManager();

	void save();
	void load(std::string fileName = "");

	std::pair<sf::Vector2f, sf::Vector2f> nextLocation(int index = -1);
	void rewriteSprites(std::vector<std::shared_ptr<Sprite>>* sprs);
	void drawMap(int layer);
	Map* getNowMap();
private:
	int mapNumber;
	Map* nowMap;
	sf::RenderWindow* window;

	std::pair<sf::Vector2f, sf::Vector2f> generate();
	std::pair<sf::Vector2f, sf::Vector2f> findStEnd(std::vector<Leaf*> leafs);
	void writeRoom(sf::IntRect rect, int layer, int value);
	void writeEnemy(std::vector<sf::IntRect> rooms);
};

static std::vector<std::string> mapFileNames{
	"base1.map",
	"arena1.map",
	"arena2.map",
	"arena3.map",
	"boss.map" };

#endif // !MAPM