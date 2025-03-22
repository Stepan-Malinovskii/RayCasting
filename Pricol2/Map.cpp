#include "Map.h"

void Map::Draw(sf::RenderTarget& target, int layerNumber)
{
	if (grid.empty()) return;

	sf::RectangleShape cell(sf::Vector2f(TEXTURE_SIZE * 0.95, TEXTURE_SIZE * 0.95));
	cell.setTexture(&Resources::textures);

	if (layerNumber != SPRITE_LAYER)
	{
		for (int y = 0; y < grid.size(); y++)
		{
			for (int x = 0; x < grid[y].size(); x++)
			{
				int init = grid[y][x][layerNumber];
				if (init != 0)
				{
					cell.setTextureRect(sf::IntRect(sf::Vector2i((init - 1) % TEXTURE_COUNT * TEXTURE_SIZE,
						(init - 1) / TEXTURE_COUNT * TEXTURE_SIZE),
						sf::Vector2i(TEXTURE_SIZE, TEXTURE_SIZE)));
					cell.setPosition((float)TEXTURE_SIZE * (sf::Vector2f(x, y) + sf::Vector2f(0.025, 0.025)));
					target.draw(cell);
				}
			}
		}
	}
	else
	{
		cell.setFillColor(sf::Color(255, 255, 255, 100));
		for (int y = 0; y < grid.size(); y++)
		{
			for (int x = 0; x < grid[y].size(); x++)
			{
				int init = grid[y][x][WALL_LAYER];
				if (init != 0)
				{
					cell.setTextureRect(sf::IntRect(sf::Vector2i((init - 1) % TEXTURE_COUNT * TEXTURE_SIZE,
						(init - 1) / TEXTURE_COUNT * TEXTURE_SIZE),
						sf::Vector2i(TEXTURE_SIZE, TEXTURE_SIZE)));
					cell.setPosition((float)TEXTURE_SIZE * (sf::Vector2f(x, y) + sf::Vector2f(0.025, 0.025)));
					target.draw(cell);
				}
			}
		}

		sf::RectangleShape spShape(sf::Vector2f(ICON_SIZE, ICON_SIZE));
		spShape.setTexture(&Resources::spriteIcon);

		for (auto sp : sprites)
		{
			if (sp.spriteDefId != 0)
			{
				spShape.setTextureRect(sf::IntRect(sf::Vector2i(ICON_SIZE * (sp.spriteDefId - 1), 0),
					sf::Vector2i(ICON_SIZE, ICON_SIZE)));
				spShape.setPosition((float)TEXTURE_SIZE * sp.position);
				target.draw(spShape);
			}
		}
	}
}

void Map::SaveGrid(const std::string& path)
{
	std::ofstream out{ path, std::ios::out | std::ios::binary };
	if (!out.is_open()) return;
	if (grid.empty()) return;

	int layer = WALL_LAYER;
	int h = grid.size();
	int w = grid[0].size();
	out.write(reinterpret_cast<const char*>(&w), sizeof(w));
	out.write(reinterpret_cast<const char*>(&h), sizeof(h));

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			out.write(reinterpret_cast<const char*>(grid[y][x].data()),
				sizeof(grid[y][x][0]) * LAYER_COUNT);
		}
	}

	int numSp = sprites.size();
	out.write(reinterpret_cast<const char*>(&numSp), sizeof(numSp));
	for (int i = 0; i < sprites.size(); i++)
		out.write(reinterpret_cast<const char*>(&sprites[i]), sizeof(sprites[i]));
	out.close();
}

void Map::LoadGrid(const std::string& path)
{
	std::ifstream in{ path, std::ios::in | std::ios::binary };
	if (!in.is_open()) return;
	int h = 0, w = 0;
	in.read(reinterpret_cast<char*>(&w), sizeof(w));
	in.read(reinterpret_cast<char*>(&h), sizeof(h));
	if (h == 0 && w == 0)
	{
		return;
	}
	
	grid = std::vector(h, std::vector(w, std::array<int, LAYER_COUNT>()));
	blockMap = std::vector(h, std::vector(w, std::set<Sprite*>()));
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			in.read(reinterpret_cast<char*>(grid[y][x].data()),
				sizeof(grid[y][x][0]) * LAYER_COUNT);
		}
	}
	
	int numSp;
	in.read(reinterpret_cast<char*>(&numSp), sizeof(numSp));

	sprites.resize(numSp);
	for (int i = 0; i < sprites.size(); i++)
		in.read(reinterpret_cast<char*>(&sprites[i]), sizeof(sprites[i]));
}

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
	sf::Vector2i start = static_cast<sf::Vector2i>(sp->spMap.position - halfSize);
	sf::Vector2i end = static_cast<sf::Vector2i>(sp->spMap.position + halfSize);

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

void Map::writeMapSprite(std::vector<std::shared_ptr<Sprite>> sprs)
{
	sprites.clear();
	MapSprite mapSp;
	for (auto sp : sprs)
	{
		sprites.push_back(sp->spMap);
	}
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