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
	template<typename Fn> ThinkerLogic(const Fn& _fn): fn{ _fn } {}

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
	int indexTexture;
	bool isDirectional;
};

class Sprite
{
public:
	Sprite(sf::Vector2f pos, float size, int indText, float angle = 0.0f, bool isDirect = false, float HP = 100.0f, SpriteType type = Enemy);
	Sprite(SpriteDef spDef, MapSprite spMap);
	Sprite() = default;

	void move(Map& map, sf::Vector2f move);
	void setupBlockmap(Map& map);
	sf::Vector2f position;
	int texture;
	SpriteType type;
	bool isDirectional;
	float angle, size, healPoint, maxHealpoint;

	std::shared_ptr<Thinker> thinker;

private:
	bool checkCollision(const Map& map, sf::Vector2f newPos, bool xAxis);
	std::set<std::tuple<int,int>> blockmap_coords;
};

static std::vector<SpriteDef> spriteDef = {
	{ "player", 0.3f, 100.0f, -1, false},
	{ "shar", 1.0f, 100.f, 0, true },
	{ "rog", 1.0f, 100.f, 1, true },
	{ "gorb", 1.0f, 100.f, 2, true },
	{ "kozel", 1.0f, 100.f, 3, true },
	{ "robot", 1.0f, 100.f, 4, true},
	{ "bachka", 1.0f, 100.f, 5, true},
	{ "pyshka", 1.0f, 100.f, 6, true} };
#endif // !SPRITE
