#pragma once
#ifndef MAP_H
#define MAP_H

#include "Resources.h"
#include "Sprite.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <fstream>
#include <array>
#include <ios>
#include <string>
#include <vector>
#include <set>

constexpr int ALL_LAYER = 4;
constexpr int LAYER_COUNT = 3;
constexpr int FLOOR_LAYER = 0;
constexpr int WALL_LAYER = 1;
constexpr int CELL_LAYER = 2;
constexpr int SPRITE_LAYER = 3;

class Map
{
public:
	void SetNewOnGrid(int x, int y, int layerNumber, int value);
	int const GetOnGrid(int x, int y, int layerNumber) const;

	bool isCellEmpty(sf::Vector2i pos);
	void setupBlockmap(Sprite* sp);
	void deleteInBlockMap(Sprite* sp);
	std::set<Sprite*> getBlockMap(sf::Vector2i pos) const;

	void setMapSprite(MapSprite sp);
	void deleteMapSprite(sf::Vector2i pos);
	std::vector<MapSprite>& getMapSprites();

	void deleteSprite(sf::Vector2i mapPos);

	std::vector<std::vector<std::array<int, LAYER_COUNT>>> grid;
	std::vector<std::vector<std::set<Sprite*>>> blockMap;

	std::vector<MapSprite> sprites;
private:
	bool insertInBlockMap(sf::Vector2i pos, Sprite* sprite);
	void removeInBlockMap(sf::Vector2i pos, Sprite* sprite);
};

#endif // !MAP_H