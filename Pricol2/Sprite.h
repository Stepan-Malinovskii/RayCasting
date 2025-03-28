
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
constexpr int ENEMY_COUNT = 14;

class Map;
class Sprite;
class Dialog;

enum SpriteType
{
	Enemy, PlayerT, NPC
};

class Thinker
{
public:
	virtual void update(Sprite*, Map*, float) = 0;
};

class ThinkerLogic : public Thinker
{
public:
	template<typename Fn> ThinkerLogic(const Fn& _fn) : moveLogic{ _fn } {}

	void update(Sprite* sprite, Map* map, float deltaT) override
	{
		moveLogic(sprite, map, deltaT);
	}
private:
	std::function<void(Sprite*, Map*, float)> moveLogic;
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
	Sprite(sf::Vector2f pos, float size, int indText, int id, float HP, 
		float angle = 0.0f, bool isDirect = false, SpriteType type = SpriteType::PlayerT);
	Sprite(SpriteDef spDef, MapSprite spMap, int _id);
	Sprite() = default;
	virtual ~Sprite() = default;

	void move(Map* map, sf::Vector2f move);
	void takeDamage(float damage);
	void update(float deltaTime);
	int id;
	SpriteDef spDef;
	MapSprite spMap;

	sf::Texture* texture;

	bool isDamages;
	int textSize;
	std::shared_ptr<Thinker> thinker;
	std::set<std::tuple<int, int>> blockmap_coords;

private:
	float timeAtecked;
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

static std::vector<SpriteDef> spriteDef = {
	{ L"player", SpriteType::PlayerT, 0.3f, 100.0f, -1, false},
	{ L"Бабл", SpriteType::Enemy, 1.0f, 70.f, 0, true },
	{ L"Рогастик", SpriteType::Enemy, 1.0f, 90.f, 1, true },
	{ L"Розовый пинки", SpriteType::Enemy, 1.0f, 100.f, 2, true },
	{ L"Кибер демон", SpriteType::Enemy, 1.0f, 120.f, 3, true },
	{ L"Спайдер", SpriteType::Enemy, 1.0f, 200.f, 4, true},
	{ L"Красный череп", SpriteType::Enemy, 1.0f, 150.f, 5, true},
	{ L"Бомбастик", SpriteType::Enemy, 1.0f, 300.f, 6, true},
	{ L"Зеленый пинки", SpriteType::Enemy, 1.0f, 200.f, 7, true },
	{ L"Синий череп", SpriteType::Enemy, 1.0f, 200.f, 8, true },
	{ L"Ревенант", SpriteType::Enemy, 1.0f, 180.f, 9, true },
	{ L"Мега бабл", SpriteType::Enemy, 1.0f, 300.f, 10, true },
	{ L"Мать", SpriteType::Enemy, 1.0f, 320.f, 11, true },
	{ L"Boss", SpriteType::Enemy, 1.0f, 2000.f, 12, true },
	{ L"Петрович", SpriteType::NPC, 1.0f, 1000000.f, 13, true },
	{ L"Молотов", SpriteType::NPC, 1.0f, 1000000.f, 14, true }, 
	{ L"Роман", SpriteType::NPC, 1.0f, 1000000.f, 15, true }, 
	{ L"Ванька", SpriteType::NPC, 1.0f, 1000000.f, 16, true }, 
	{ L"Тихон", SpriteType::NPC, 1.0f, 1000000.f, 17, true }, 
	{ L"Виктор", SpriteType::NPC, 1.0f, 1000000.f, 18, true }};
#endif // !SPRITE
