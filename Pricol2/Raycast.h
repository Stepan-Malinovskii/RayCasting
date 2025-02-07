#pragma once
#ifndef RAYCAST
#define RAYCAST

#include "Map.h"

struct RayHit
{
	int cell;
	sf::Vector2i mapPos;
	bool isHitVert;
	float perpWallDist;

	Sprite* sprite;
};

RayHit raycast(const Map& map, sf::Vector2f pos, sf::Vector2f dir, 
	bool checkSprite = false, Sprite* ignore = nullptr, int maxDist = 64);

#endif // !RAYCAST