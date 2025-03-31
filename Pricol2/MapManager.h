#pragma once
#ifndef MAPM
#define MAPM
#include "Map.h"
#include "Resources.h"
#include <SFML/Graphics/RenderWindow.hpp>

struct LoadData
{
	int trigerNumber;
	std::string pathName;
};

class MapManager
{
public:
	MapManager(sf::RenderWindow* _window);
	~MapManager();

	void save();
	void load();
	void rewriteSprites(std::vector<std::shared_ptr<Sprite>> sprs);
	void drawMap(int layer);
	Map* getNowMap();
private:
	Map* nowMap;
	sf::RenderWindow* window;
};

static std::vector<LoadData> loadData{
	{0, "test.map"},
	{1, ""},
	{5, ""}};
#endif // !MAPM