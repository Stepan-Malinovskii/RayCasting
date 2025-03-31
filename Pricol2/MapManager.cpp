#include "MapManager.h"

MapManager::MapManager(sf::RenderWindow* _window) : window{ _window }{}

MapManager::~MapManager()
{
	delete nowMap;
}

void MapManager::save()
{
	std::ofstream out{ "Texture/testtest.map", std::ios::out | std::ios::binary};
	if (!out.is_open()) return;
	if (nowMap->grid.empty()) return;

	auto grid = nowMap->grid;
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

	auto sprites = nowMap->sprites;
	int numSp = sprites.size();
	out.write(reinterpret_cast<const char*>(&numSp), sizeof(numSp));
	for (int i = 0; i < sprites.size(); i++)
		out.write(reinterpret_cast<const char*>(&sprites[i]), sizeof(sprites[i]));
	out.close();
}

void MapManager::load()
{
	std::ifstream in{ "Texture/testtest.map", std::ios::in | std::ios::binary};
	if (!in.is_open()) return;
	nowMap = new Map();

	int h = 0, w = 0;
	in.read(reinterpret_cast<char*>(&w), sizeof(w));
	in.read(reinterpret_cast<char*>(&h), sizeof(h));

	if (h == 0 && w == 0) { return; }

	nowMap->grid = std::vector(h, std::vector(w, std::array<int, LAYER_COUNT>()));
	nowMap->blockMap = std::vector(h, std::vector(w, std::set<Sprite*>()));

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			in.read(reinterpret_cast<char*>(nowMap->grid[y][x].data()),
				sizeof(nowMap->grid[y][x][0]) * LAYER_COUNT);
		}
	}

	int numSp;
	in.read(reinterpret_cast<char*>(&numSp), sizeof(numSp));

	nowMap->sprites = std::vector<MapSprite>(numSp);
	for (int i = 0; i < nowMap->sprites.size(); i++)
		in.read(reinterpret_cast<char*>(&nowMap->sprites[i]), sizeof(nowMap->sprites[i]));

	in.close();
}

void MapManager::rewriteSprites(std::vector<std::shared_ptr<Sprite>> sprs)
{
	nowMap->sprites.clear();
	for (auto sp : sprs)
	{
		nowMap->sprites.push_back(sp->spMap);
	}
}

void MapManager::drawMap(int layerNumber)
{
	if (nowMap->grid.empty()) return;

	sf::RectangleShape cell(sf::Vector2f(TEXTURE_SIZE * 0.95, TEXTURE_SIZE * 0.95));
	cell.setTexture(&Resources::textures);

	if (layerNumber != SPRITE_LAYER)
	{
		for (int y = 0; y < nowMap->grid.size(); y++)
		{
			for (int x = 0; x < nowMap->grid[y].size(); x++)
			{
				int init = nowMap->grid[y][x][layerNumber];
				if (init != 0)
				{
					cell.setTextureRect(sf::IntRect(sf::Vector2i((init - 1) % TEXTURE_COUNT * TEXTURE_SIZE,
						(init - 1) / TEXTURE_COUNT * TEXTURE_SIZE),
						sf::Vector2i(TEXTURE_SIZE, TEXTURE_SIZE)));
					cell.setPosition((float)TEXTURE_SIZE * (sf::Vector2f(x, y) + sf::Vector2f(0.025, 0.025)));
					window->draw(cell);
				}
			}
		}
	}
	else
	{
		cell.setFillColor(sf::Color(255, 255, 255, 100));
		for (int y = 0; y < nowMap->grid.size(); y++)
		{
			for (int x = 0; x < nowMap->grid[y].size(); x++)
			{
				int init = nowMap->grid[y][x][WALL_LAYER];
				if (init != 0)
				{
					cell.setTextureRect(sf::IntRect(sf::Vector2i((init - 1) % TEXTURE_COUNT * TEXTURE_SIZE,
						(init - 1) / TEXTURE_COUNT * TEXTURE_SIZE),
						sf::Vector2i(TEXTURE_SIZE, TEXTURE_SIZE)));
					cell.setPosition((float)TEXTURE_SIZE * (sf::Vector2f(x, y) + sf::Vector2f(0.025, 0.025)));
					window->draw(cell);
				}
			}
		}

		sf::RectangleShape spShape(sf::Vector2f(ICON_SIZE, ICON_SIZE));
		spShape.setTexture(&Resources::spriteIcon);

		for (auto sp : nowMap->sprites)
		{
			if (sp.spriteDefId != 0)
			{
				spShape.setTextureRect(sf::IntRect(sf::Vector2i(ICON_SIZE * (sp.spriteDefId - 1), 0),
					sf::Vector2i(ICON_SIZE, ICON_SIZE)));
				spShape.setPosition((float)TEXTURE_SIZE * sp.position);
				window->draw(spShape);
			}
		}
	}
}

Map* MapManager::getNowMap() { return nowMap; }
