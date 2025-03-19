
#pragma once
#ifndef SPRITE
#define SPRITE

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Texture.hpp"

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
	template<typename Fn> ThinkerLogic(const Fn& _fn) : fn{ _fn } {}

	void update(Sprite& sprite, Map& map, float deltaT) override
	{
		fn(sprite, map, deltaT);
	}
private:
	std::function<void(Sprite&, Map&, float)> fn;
};

enum SpriteType
{
	Enemy, Thing, NPC
};

struct MapSprite
{
	int spriteDefId;
	sf::Vector2f position;
	float angle;
	float nowHealPoint;
};

struct SpriteDef
{
	std::string name;
	SpriteType type;
	float size;
	float maxHealpoint;
	int texture;
	bool isDirectional;
};

class Sprite
{
public:
	Sprite(sf::Vector2f pos, float size, int indText, int id, float HP, float angle = 0.0f, bool isDirect = false, SpriteType type = SpriteType::Thing);
	Sprite(SpriteDef spDef, MapSprite spMap, int _id);
	Sprite() = default;

	void move(Map* map, sf::Vector2f move);
	int id;
	SpriteDef spDef;
	MapSprite spMap;

	sf::Texture* texture;
	int textSize;

	std::shared_ptr<Thinker> thinker;
	std::set<std::tuple<int, int>> blockmap_coords;

private:
	bool checkCollision(const Map& map, sf::Vector2f newPos, bool xAxis);
};

static std::vector<SpriteDef> spriteDef = {
	{ "player", SpriteType::Thing, 0.3f, 100.0f, -1, false},
	{ "shar", SpriteType::Enemy, 1.0f, 100.f, 0, true },
	{ "rog", SpriteType::Enemy, 1.0f, 100.f, 1, true },
	{ "gorb", SpriteType::Enemy, 1.0f, 100.f, 2, true },
	{ "kozel", SpriteType::Enemy, 1.0f, 100.f, 3, true },
	{ "robot", SpriteType::Enemy, 1.0f, 100.f, 4, true},
	{ "bachka", SpriteType::Enemy, 1.0f, 100.f, 5, true},
	{ "pyshka", SpriteType::Enemy, 1.0f, 100.f, 6, true},
	{ "gorbGreen", SpriteType::Enemy, 1.0f, 100.f, 7, true },
	{ "bachkaSin", SpriteType::Enemy, 1.0f, 100.f, 8, true },
	{ "revenant", SpriteType::Enemy, 1.0f, 100.f, 9, true },
	{ "sharTre", SpriteType::Enemy, 1.0f, 100.f, 10, true },
	{ "mather", SpriteType::Enemy, 1.0f, 100.f, 11, true },
	{ "Boss", SpriteType::Enemy, 1.0f, 1000.f, 12, true }};
#endif // !SPRITE
