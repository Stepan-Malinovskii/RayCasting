#pragma once
#ifndef RANDOM
#define RANDOM

#include <random>
#include <set>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "sfmlExtantion.h"

class Random
{
public:
	static float bitRandom();
	static int intRandom(int, int);
	static std::set<sf::Vector2i, Vector2iCompare> uniquePoints(sf::IntRect zone, int count);
};
#endif // !RANDOM