#include "MapManager.h"

MapManager::MapManager(sf::RenderWindow* _window) : window{ _window }{}

MapManager::~MapManager()
{
	delete nowMap;
}

void MapManager::save()
{
	std::ofstream out{ "Data/current.map", std::ios::out | std::ios::binary};
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
	std::ifstream in{ "Data/current.map", std::ios::in | std::ios::binary};
	if (!in.is_open()) return;
	nowMap = new Map();

	int h = 0, w = 0;
	in.read(reinterpret_cast<char*>(&w), sizeof(w));
	in.read(reinterpret_cast<char*>(&h), sizeof(h));

	if (h == 0 && w == 0)
	{
		h = SPACE_SIZE_H;
		w = SPACE_SIZE_W;
	}

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

	int numSp = 0;
	in.read(reinterpret_cast<char*>(&numSp), sizeof(numSp));

	nowMap->sprites = std::vector<MapSprite>(numSp);
	for (int i = 0; i < nowMap->sprites.size(); i++)
		in.read(reinterpret_cast<char*>(&nowMap->sprites[i]), sizeof(nowMap->sprites[i]));

	in.close();

	//SoundManager::playSound(Resources::backgroundSound[0], 20, {nowMap->grid[0].size() / 2.0f, nowMap->grid.size() / 2.0f }, true, true);
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
				spShape.setOrigin(spShape.getLocalBounds().width / 2, spShape.getLocalBounds().height / 2);
				spShape.setPosition((float)TEXTURE_SIZE * sp.position);
				spShape.rotate(sp.angle);
				window->draw(spShape);
			}
		}
	}
}

std::pair<sf::Vector2f, sf::Vector2f> MapManager::generate()
{
	Leaf* root = new Leaf({ 0,0 }, { SPACE_SIZE_W, SPACE_SIZE_H });
	std::vector<Leaf*> tempLeaf;
	tempLeaf.push_back(root);

	bool didSplit = true;
	while (didSplit)
	{
		didSplit = false;
		for (int i = 0; i < tempLeaf.size(); i++)
		{
			if (tempLeaf[i]->leftChild == nullptr && tempLeaf[i]->rightChild == nullptr)
			{
				if (tempLeaf[i]->leafData.width > MAX_LEAF_SIZE || tempLeaf[i]->leafData.height > MAX_LEAF_SIZE || Random::bitRandom() > 0.25)
				{
					if (tempLeaf[i]->split())
					{
						tempLeaf.push_back(tempLeaf[i]->leftChild);
						tempLeaf.push_back(tempLeaf[i]->rightChild);
						didSplit = true;
					}
				}
			}
		}
	}

	root->creatRooms();

	std::vector<Leaf*> leafs;
	std::vector<sf::IntRect> rooms;
	std::vector<sf::IntRect> halls;

	for (auto l : tempLeaf)
	{
		if (l->isRoom)
		{
			leafs.push_back(l);
			rooms.push_back(l->leafData);
		}

		for (auto h : l->halls)
		{
			halls.push_back(h);
		}
	}

	auto stEnd = findStEnd(leafs);
	delete root;
	delete nowMap;

	nowMap = new Map();
	nowMap->grid = std::vector(SPACE_SIZE_H, std::vector(SPACE_SIZE_W, std::array<int, LAYER_COUNT>()));
	nowMap->blockMap = std::vector(SPACE_SIZE_H, std::vector(SPACE_SIZE_W, std::set<Sprite*>()));
	
	std::vector<sf::IntRect> enemyRooms = rooms;

	for (int i = 0; i < enemyRooms.size(); i++)
	{
		if (enemyRooms[i].contains((sf::Vector2i)stEnd.first))
		{
			enemyRooms.erase(enemyRooms.begin() + i);
			break;
		}
	}

	writeEnemy(enemyRooms);

	for (auto rect : rooms)
	{
		for (auto i : std::vector<std::pair<int, int>>{ {FLOOR_LAYER, 1}, {CELL_LAYER, 2} })
		{
			writeRoom(rect, i.first, Random::intRandom(1, TEXTURE_COUNT) + TEXTURE_COUNT * i.second);
		}

		sf::IntRect smalRect({ rect.left + 1, rect.top + 1 }, { rect.width - 2, rect.height - 2 });
		writeRoom(rect, 1, Random::intRandom(2, TEXTURE_COUNT));
		writeRoom(smalRect, 1, 0);
	}

	for (int y = 0; y < nowMap->grid.size(); y++)
	{
		for (int x = 0; x < nowMap->grid[0].size(); x++)
		{
			if (nowMap->grid[y][x][1] > 1)
			{
				if (Random::bitRandom() > 0.9f)
				{
					nowMap->grid[y][x][1] = Random::intRandom(1, TEXTURE_COUNT) + TEXTURE_COUNT * 3;
				}
			}
		}
	}

	for (auto h : halls)
	{
		writeRoom(h, 1, 0);
		if (h.width > h.height)
		{
			nowMap->grid[h.top][h.left + 1][1] = 1;
		}
		else
		{
			nowMap->grid[h.top + 1][h.left][1] = 1;
		}
	}


	return stEnd;
}

std::pair<sf::Vector2f, sf::Vector2f> MapManager::findStEnd(std::vector<Leaf*> leafs)
{
	std::vector<sf::Vector2f> midPoint;
	for (auto l : leafs)
	{
		auto p = l->leafData;
		midPoint.push_back({ p.left + p.width / 2.0f,
							p.top + p.height / 2.0f });
	}

	auto getDist = [](sf::Vector2f p1, sf::Vector2f p2)
		{ return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2); };

	float maxDist = 0;
	std::pair<sf::Vector2f, sf::Vector2f> result;
	for (int i = 0; i < midPoint.size(); i++)
	{
		for (int j = i + 1; j < midPoint.size(); j++)
		{
			auto dist = getDist(midPoint[i], midPoint[j]);
			if (maxDist < dist)
			{
				maxDist = dist;
				result.first = midPoint[i];
				result.second = midPoint[j];
			}
		}
	}
	return result;
}

void MapManager::writeRoom(sf::IntRect rect, int layer, int value)
{
	for (int y = rect.top; y < rect.top + rect.height; y++)
	{
		for (int x = rect.left; x < rect.left + rect.width; x++)
		{
			nowMap->grid[y][x][layer] = value;
		}
	}
}

void MapManager::writeEnemy(std::vector<sf::IntRect> rooms)
{
	int midleRoomCount = ENEMY_LEVEL_COUNT / rooms.size();

	for (auto r : rooms)
	{
		r.top += 2;
		r.left += 2;
		r.height -= 4;
		r.width -= 4;
		auto points = Random::uniquePoints(r, Random::intRandom(midleRoomCount * 0.8f, 
																midleRoomCount * 1.2f));
		for (auto p : points)
		{
			auto def = spriteDef[Random::intRandom(1, ENEMY_COUNT - 2)];
			nowMap->setMapSprite({def.texture + 1, (sf::Vector2f)p, (float)Random::intRandom(0,180), def.maxHealpoint});
		}
	}
}

Map* MapManager::getNowMap() { return nowMap; }
