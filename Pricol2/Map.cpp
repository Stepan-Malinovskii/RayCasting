#include "Map.h"

void Map::Draw(sf::RenderTarget& target, int layerNumber)
{
	if (grid.empty()) return;

	sf::RectangleShape cell(sf::Vector2f(TEXTURE_SIZE * 0.95, TEXTURE_SIZE * 0.95));
	cell.setTexture(&Resources::textures);

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

	std::vector<MapSprite> sprites =
	{
		{0, {2.0f, 2.0f}, 0},
		{1, {2.5f, 2.5f}, 0},
		{2, {6.5f, 6.5f}, 0},
		{3, {8.5f, 8.5f}, 0},
		{4, {7.5f, 10.5f}, 0}
	};
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

int const Map::GetNewOnGrid(int x, int y, int layerNumber) const
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

const std::vector<MapSprite>& Map::getSprites() const
{
	return sprites;
}