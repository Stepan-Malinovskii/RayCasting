#pragma once
#ifndef MAPM
#define MAPM

#include "Map.h"
#include "Leaf.h"
#include "Resources.h"
#include "SoundManager.h"
#include <SFML/Graphics/RenderWindow.hpp>

constexpr int ENEMY_LEVEL_COUNT = 40;

class MapManager
{
public:
	MapManager(sf::RenderWindow* _window);
	~MapManager();

	void save();
	void load();
	std::pair<sf::Vector2f, sf::Vector2f> generate();
	void rewriteSprites(std::vector<Sprite*> sprs);
	void drawMap(int layer);
	Map* getNowMap();
private:
	Map* nowMap;
	sf::RenderWindow* window;

	std::pair<sf::Vector2f, sf::Vector2f> findStEnd(std::vector<Leaf*> leafs);
	void writeRoom(sf::IntRect rect, int layer, int value);
	void writeEnemy(std::vector<sf::IntRect> rooms);
};
#endif // !MAPM