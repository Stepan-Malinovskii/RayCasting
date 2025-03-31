#include "Map.h"

void Map::SetNewOnGrid(int x, int y, int layerNumber, int value)
{
	if (x >= 0 && y >= 0 && y < grid.size() && x < grid[y].size())
		grid[y][x][layerNumber] = value;
}

int const Map::GetOnGrid(int x, int y, int layerNumber) const
{
	if (y >= 0 && y < grid.size() && x >= 0 && x < grid[y].size())
		return grid[y][x][layerNumber];
	return 0;
}

bool Map::insertInBlockMap(sf::Vector2i pos, Sprite* sprite)
{
	if (pos.x >= 0 && pos.y >= 0 && pos.y < blockMap.size() && pos.x < blockMap[pos.y].size())
	{
		blockMap[pos.y][pos.x].insert(sprite);
		return true;
	}
	return false;
}

void Map::removeInBlockMap(sf::Vector2i pos, Sprite* sprite)
{
	if (pos.x >= 0 && pos.y >= 0 && pos.y < blockMap.size() && pos.x < blockMap[pos.y].size())
	{
		blockMap[pos.y][pos.x].erase(sprite);
	}
}

bool Map::isCellEmpty(sf::Vector2i pos)
{
	if (pos.x >= 0 && pos.y >= 0 && pos.y < blockMap.size() && pos.x < blockMap[pos.y].size())
	{
		return blockMap[pos.y][pos.x].empty();
	}
	return false;
}

void Map::setupBlockmap(Sprite* sp)
{
	sf::Vector2f halfSize = { sp->spDef.size / 2.f, sp->spDef.size / 2.f };
	sf::Vector2i start = (sf::Vector2i)(sp->spMap.position - halfSize);
	sf::Vector2i end = (sf::Vector2i)(sp->spMap.position + halfSize);

	std::set<std::tuple<int, int>> coords;
	for (int y = start.y; y <= end.y; y++) {
		for (int x = start.x; x <= end.x; x++) { coords.insert({ x, y }); }
	}

	std::set<std::tuple<int, int>> to_remove;
	auto blockmap_coords = sp->blockmap_coords;
	std::set_difference(blockmap_coords.begin(), blockmap_coords.end(),
		coords.begin(), coords.end(),
		std::inserter(to_remove, to_remove.end()));

	std::set<std::tuple<int, int>> to_insert;
	std::set_difference(coords.begin(), coords.end(), blockmap_coords.begin(),
		blockmap_coords.end(),
		std::inserter(to_insert, to_insert.end()));

	for (const auto& [x, y] : to_remove) { removeInBlockMap({ x, y }, sp); }
	for (const auto& [x, y] : to_insert) { insertInBlockMap({ x, y }, sp); }

	sp->blockmap_coords = coords;
}

void Map::deleteInBlockMap(Sprite* sp)
{
	for (const auto& [x, y] : sp->blockmap_coords)
	{
		blockMap[y][x].erase(sp);
	}
}

std::set<Sprite*> Map::getBlockMap(sf::Vector2i pos) const
{
	if (pos.x >= 0 && pos.y >= 0 && pos.y < blockMap.size() && pos.x < blockMap[pos.y].size())
	{
		return blockMap[pos.y][pos.x];
	}
	return {};
}

std::vector<MapSprite>& Map::getMapSprites()
{
	return sprites;
}

void Map::setMapSprite(MapSprite sp)
{
	sprites.push_back(sp);
}

void Map::deleteMapSprite(sf::Vector2i pos)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		if ((sf::Vector2i)sprites[i].position == pos)
		{
			sprites.erase(sprites.begin() + i);
			break;
		}
	}
}

void Map::deleteSprite(sf::Vector2i mapPos)
{
	if (mapPos.x >= 0 && mapPos.y >= 0 && mapPos.y < blockMap.size() && mapPos.x < blockMap[mapPos.y].size())
	{
		for (auto sp = blockMap[mapPos.y][mapPos.x].begin(); sp != blockMap[mapPos.y][mapPos.x].end();)
		{
			if ((*sp)->spDef.texture != -1)
			{
				blockMap[mapPos.y][mapPos.x].erase(sp);
				break;
			}
			else
			{
				sp++;
			}
		}

		for (int i = 0; i < sprites.size(); i++)
		{
			if ((sf::Vector2i)sprites[i].position == mapPos)
			{
				sprites.erase(sprites.begin() + i);
			}
		}
	}
}