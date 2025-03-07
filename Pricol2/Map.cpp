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
					cell.setTextureRect(sf::IntRect(sf::Vector2i(TEXTURE_SIZE * (init - 1), 0.0f),
						sf::Vector2i(TEXTURE_SIZE, TEXTURE_SIZE)));
					cell.setPosition(TEXTURE_SIZE * (sf::Vector2f(x, y) + sf::Vector2f(0.025, 0.025)));
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
					cell.setTextureRect(sf::IntRect(sf::Vector2i(TEXTURE_SIZE * (init - 1), 0.0f),
						sf::Vector2i(TEXTURE_SIZE, TEXTURE_SIZE)));
					cell.setPosition(TEXTURE_SIZE * (sf::Vector2f(x, y) + sf::Vector2f(0.025, 0.025)));
					target.draw(cell);
				}
			}
		}

		sf::RectangleShape spShape(sf::Vector2f(TEXTURE_SIZE * 0.95, TEXTURE_SIZE * 0.95));
		spShape.setTexture(&Resources::spritesTexture);

		for (auto sp : sprites)
		{
			if (sp.idx != 0)
			{
				spShape.setTextureRect(sf::IntRect(sf::Vector2i(0.0f, SPRITE_SIZE * (sp.idx - 1)),
					sf::Vector2i(SPRITE_SIZE, SPRITE_SIZE)));
				spShape.setPosition((sf::Vector2f)((int)TEXTURE_SIZE * (sf::Vector2i)sp.position));
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

void Map::insertInBlockMap(sf::Vector2i pos, Sprite* sprite)
{
	if (pos.x >= 0 && pos.y >= 0 && pos.y < blockMap.size() && pos.x < blockMap[pos.y].size())
	{
		blockMap[pos.y][pos.x].insert(sprite);
	}
}

void Map::removeInBlockMap(sf::Vector2i pos, Sprite* sprite)
{
	if (pos.x >= 0 && pos.y >= 0 && pos.y < blockMap.size() && pos.x < blockMap[pos.y].size())
	{
		blockMap[pos.y][pos.x].erase(sprite);

		for (int i = 0; i < sprites.size(); i++)
		{
			if ((sf::Vector2i)sprites[i].position == pos)
			{
				sprites.erase(sprites.begin() + i);
				break;
			}
		}
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

void Map::setSprites(Sprite& sprite)
{
	sf::Vector2i pos = (sf::Vector2i)sprite.position;
	int l = blockMap[pos.y][pos.x].size();
	insertInBlockMap((sf::Vector2i)sprite.position, &sprite);
	if (l != blockMap[pos.y][pos.x].size())
	{
		sprites.push_back({ sprite.texture + 1, sprite.position, sprite.angle });
	}
}

void Map::deleteSprite(sf::Vector2i mapPos)
{
	if (mapPos.x >= 0 && mapPos.y >= 0 && mapPos.y < blockMap.size() && mapPos.x < blockMap[mapPos.y].size())
	{
		for (auto sp = blockMap[mapPos.y][mapPos.x].begin(); sp != blockMap[mapPos.y][mapPos.x].end();)
		{
			if ((*sp)->texture != -1)
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