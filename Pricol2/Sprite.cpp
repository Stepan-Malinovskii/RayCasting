#include "Sprite.h"
#include "Map.h"

Sprite::Sprite(sf::Vector2f pos, float _size, int indText, float _angle)
	: position{ pos }, size{ _size}, texture{ indText }, angle{ _angle } 
{
	blockmap_coords = (sf::Vector2i)position;
}

void Sprite::move(Map& map, sf::Vector2f move)
{
	if (move == sf::Vector2f()) return;

	float xOffset = move.x > 0 ? size : -size;
	float yOffset = move.y > 0 ? size : -size;
	if (!checkCollision(map, { position.x + move.x + xOffset, position.y }))
	{
		position.x += move.x;
	}
	if (!checkCollision(map, { position.x, position.y + move.y + yOffset }))
	{
		position.y += move.y;
	}

	setupBlockmap(map);
}

void Sprite::setupBlockmap(Map& map)
{
	if (blockmap_coords != (sf::Vector2i)position)
	{
		map.removeInBlockMap(blockmap_coords, this);
		blockmap_coords = (sf::Vector2i)position;
		map.insertInBlockMap(blockmap_coords, this);
	}
}

bool Sprite::checkCollision(const Map& map, sf::Vector2f newPos)
{
	if (map.GetNewOnGrid(newPos.x, newPos.y, WALL_LAYER)) return true;

	auto curSp = map.getBlockMap((sf::Vector2i)newPos);

	for (auto sp : curSp)
	{
		if (sp->size == 0 || sp == this) continue;
		return true;
	}

	return false;
}
