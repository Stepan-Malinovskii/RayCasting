
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
class MapSprite;
class Sprite;

enum SpriteType
{
	Enemy, Thing
};

class Thinker
{
public:
	virtual void update(Sprite&, Map&, float) = 0;
};

class ThinkerLogic : public Thinker
{
public:
	template<typename Fn> ThinkerLogic(const Fn& _fn) : fn{ _fn } {}

	void update(Sprite& sprite, Map& map, float deltaT) override
	{
		fn(sprite, map, deltaT);
	}
private:
	std::function<void(Sprite&, Map&, float)> fn;
};

struct SpriteDef
{
	std::string name;
	float size;
	float maxHealpoint;
	int texture;
	bool isDirectional;
};

class Sprite
{
public:
	Sprite(sf::Vector2f pos, float size, int indText, int id, float HP, float angle = 0.0f, bool isDirect = false, SpriteType type = Enemy);
	Sprite(SpriteDef spDef, MapSprite spMap, int _id);
	Sprite() = default;

	void move(Map* map, sf::Vector2f move);
	sf::Vector2f position;
	int id;
	SpriteType type;
	SpriteDef spDef;
	float angle, healPoint;

	std::shared_ptr<Thinker> thinker;
	std::set<std::tuple<int, int>> blockmap_coords;

private:
	bool checkCollision(const Map& map, sf::Vector2f newPos, bool xAxis);
};

static std::vector<SpriteDef> spriteDef = {
	{ "player", 0.3f, 100.0f, -1, false},
	{ "shar", 1.0f, 100.f, 0, true },
	{ "rog", 1.0f, 100.f, 1, true },
	{ "gorb", 1.0f, 100.f, 2, true },
	{ "kozel", 1.0f, 100.f, 3, true },
	{ "robot", 1.0f, 100.f, 4, true},
	{ "bachka", 1.0f, 100.f, 5, true},
	{ "pyshka", 1.0f, 100.f, 6, true},
	{ "gorbGreen", 1.0f, 100.f, 7, true },
	{ "bachkaSin", 1.0f, 100.f, 8, true },
	{ "revenant", 1.0f, 100.f, 9, true },
	{ "sharTre", 1.0f, 100.f, 10, true },
	{ "mather", 1.0f, 100.f, 11, true },
	{ "Boss", 1.0f, 1000.f, 12, true }};
#endif // !SPRITE
