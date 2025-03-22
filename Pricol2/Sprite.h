
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
class Dialog;

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
	std::wstring name;
	SpriteType type;
	float size;
	float maxHealpoint;
	int texture;
	bool isDirectional;
};

struct NpcDef
{
	int startKey;
	int trigerKey;
};

class Sprite
{
public:
	Sprite(sf::Vector2f pos, float size, int indText, int id, float HP, float angle = 0.0f, bool isDirect = false, SpriteType type = SpriteType::Thing);
	Sprite(SpriteDef spDef, MapSprite spMap, int _id);
	Sprite() = default;
	virtual ~Sprite() = default;

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

class Npc : public Sprite
{
public:
	Npc(SpriteDef spDef, MapSprite spMap, int _id, int hisKey, Dialog* dialog);
	Npc() = default;
	void use();
	NpcDef npcDefData;
private:
	Dialog* dialog;
};

static std::vector<NpcDef> npcDef = {
	{1, 404}};

constexpr int ENEMY_COUNT = 14;
static std::vector<SpriteDef> spriteDef = {
	{ L"player", SpriteType::Thing, 0.3f, 100.0f, -1, false},
	{ L"shar", SpriteType::Enemy, 1.0f, 100.f, 0, true },
	{ L"rog", SpriteType::Enemy, 1.0f, 100.f, 1, true },
	{ L"gorb", SpriteType::Enemy, 1.0f, 100.f, 2, true },
	{ L"kozel", SpriteType::Enemy, 1.0f, 100.f, 3, true },
	{ L"robot", SpriteType::Enemy, 1.0f, 100.f, 4, true},
	{ L"bachka", SpriteType::Enemy, 1.0f, 100.f, 5, true},
	{ L"pyshka", SpriteType::Enemy, 1.0f, 100.f, 6, true},
	{ L"gorbGreen", SpriteType::Enemy, 1.0f, 100.f, 7, true },
	{ L"bachkaSin", SpriteType::Enemy, 1.0f, 100.f, 8, true },
	{ L"revenant", SpriteType::Enemy, 1.0f, 100.f, 9, true },
	{ L"sharTre", SpriteType::Enemy, 1.0f, 100.f, 10, true },
	{ L"mather", SpriteType::Enemy, 1.0f, 100.f, 11, true },
	{ L"Boss", SpriteType::Enemy, 1.0f, 1000.f, 12, true },
	{ L"Петрович", SpriteType::NPC, 1.0f, 1000000.f, 13, true },
	{ L"Молотов", SpriteType::NPC, 1.0f, 1000000.f, 14, true }, 
	{ L"Роман", SpriteType::NPC, 1.0f, 1000000.f, 15, true }, 
	{ L"Ванька", SpriteType::NPC, 1.0f, 1000000.f, 16, true }, 
	{ L"Тихон", SpriteType::NPC, 1.0f, 1000000.f, 17, true }, 
	{ L"Виктор", SpriteType::NPC, 1.0f, 1000000.f, 18, true }};
#endif // !SPRITE
