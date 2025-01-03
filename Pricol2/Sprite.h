#pragma once
#ifndef SPRITE
#define SPRITE

#include "SFML/System/Vector2.hpp"

#include <functional>
#include <algorithm>
#include <memory>
#include <set>
#include <iostream>

constexpr float PI = 3.14159265359f;

class Map;
class Sprite;

class Thinker
{
public:
	virtual void update(Sprite&, Map&, float) = 0;
};

class ThinkerLogic : public Thinker
{
public:
	template<typename Fn> ThinkerLogic(const Fn& _fn): fn{ _fn } {}

	void update(Sprite& sprite, Map& map, float deltaT) override
	{
		fn(sprite, map, deltaT);
	}
private:
	std::function<void(Sprite&, Map&, float)> fn;
};

class Sprite
{
public:
	Sprite(sf::Vector2f pos, float size, int indText, float angle = 0.0f);

	void move(Map& map, sf::Vector2f move);
	void setupBlockmap(Map& map);
	sf::Vector2f position;
	int texture;
	float angle, size;

	std::shared_ptr<Thinker> thinker;

private:
	bool checkCollision(const Map& map, sf::Vector2f newPos);
	sf::Vector2i blockmap_coords;
};

struct SpriteDef
{
	std::string name;
	float size;
	int indexTexture;
};

static std::vector<SpriteDef> spriteDef = {
	{ "player", 0.3f, -1 },
	{ "shar", 1.0f, 0 },
	{ "rog", 1.0f, 1 },
	{ "gorb", 1.0f, 2 },
	{ "kozel", 1.0f, 3 }};
#endif // !SPRITE
