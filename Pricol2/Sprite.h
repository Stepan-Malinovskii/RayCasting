#pragma once
#ifndef SPRITE
#define SPRITE

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <functional>
#include <algorithm>
#include "Animation.h"
#include <memory>
#include <set>
#include <iostream>

constexpr float PI = 3.14159265359f, TRIGER_DIST_MAX = 80, TRIGER_DIST_MIN = 40;
constexpr int ENEMY_COUNT = 14;

class Map;
class Sprite;
class Dialog;

enum class SpriteType
{
	Enemy, PlayerT, NPC
};

enum SpriteState
{
	Stay, Run, Atack, Killes, Dead
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
	int texture;
	bool isDirectional;
};

struct EnemyDef
{
	bool isCanRun;
	float atackDist;
	float damage;
	int midleDrop;
	float timeBettwenAtack;
	float speed;
	float maxHealpoint;
};

struct NpcDef
{
	int startKey;
};

class Sprite
{
public:
	Sprite(SpriteDef spDef, MapSprite spMap, int _id);
	Sprite() = default;
	virtual ~Sprite() = default;
	virtual std::pair<int, bool> getTextIndex();

	int id;
	int textSize;
	SpriteDef spDef;
	MapSprite spMap;
	sf::Texture* texture;
	std::set<std::tuple<int, int>> blockmap_coords;
};

class Enemy : public Sprite
{
public:
	Enemy(SpriteDef spDef, MapSprite spMap, EnemyDef enemyDef, int id);
	~Enemy() = default;
	std::pair<int, bool> getTextIndex() override;
	void update(float deltaTime);
	void move(Map* map, sf::Vector2f move);
	void takeDamage(float damage);
	bool changeState(SpriteState state);

	EnemyDef enemyDef;
	SpriteState state;
private:
	float timeAtecked, nowTimeAtack;
	bool isDamages;
	Animator<int> animr;
	bool checkCollision(Map* map, sf::Vector2f newPos, bool xAxis);
};

class Npc : public Sprite
{
public:
	Npc(SpriteDef spDef, MapSprite spMap, int _id, int npcId, Dialog* dialog);
	Npc() = default;
	void use();
	NpcDef npcDefData;
private:
	Dialog* dialog;
};

static std::vector<NpcDef> npcDef = {
	{1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1} };

static std::vector<EnemyDef> enemyDef = {
	{true,  0.0f,  0.0f, 0,  0.0f, 5.0f, 0.0f },
	{true,  5.0f,  1.0f, 10, 3.0f, 3.0f, 70.f },
	{true,  5.0f,  1.0f, 15, 3.0f, 4.0f, 90.f },
	{true,  5.0f,  1.0f, 20, 3.0f, 5.0f, 100.f},
	{true,  20.0f, 1.0f, 25, 3.0f, 4.0f, 120.f},
	{true,  20.0f, 1.0f, 30, 3.0f, 5.0f, 200.f},
	{true,  5.0f,  1.0f, 35, 3.0f, 6.0f, 150.f},
	{true,  20.0f, 1.0f, 40, 3.0f, 3.0f, 300.f},
	{true,  5.0f,  1.0f, 45, 3.0f, 6.0f, 200.f},
	{true,  5.0f,  1.0f, 50, 3.0f, 6.0f, 200.f},
	{true,  20.0f, 1.0f, 55, 3.0f, 5.0f, 180.f},
	{false, 20.0f, 1.0f, 60, 3.0f, 4.0f, 300.f},
	{true,  5.0f,  1.0f, 65, 3.0f, 4.0f, 320.f},
	{true,  5.0f,  1.0f, 10, 3.0f, 5.0f, 2000.f}
};

static std::vector<SpriteDef> spriteDefs = {
	{ L"player",        SpriteType::PlayerT, 0.3f, -1,  false},
	{ L"Бабл",          SpriteType::Enemy,   1.0f,  0,  true},
	{ L"Синчик",        SpriteType::Enemy,   1.0f,  1,  true},
	{ L"Розовый пинки", SpriteType::Enemy,   1.0f,  2,  true},
	{ L"Кибер демон",   SpriteType::Enemy,   1.0f,  3,  true},
	{ L"Спайдер",       SpriteType::Enemy,   1.0f,  4,  true},
	{ L"Красный череп", SpriteType::Enemy,   1.0f,  5,  true},
	{ L"Бомбастик",     SpriteType::Enemy,   1.0f,  6,  true},
	{ L"Зеленый пинки", SpriteType::Enemy,   1.0f,  7,  true},
	{ L"Розовый череп", SpriteType::Enemy,   1.0f,  8,  true},
	{ L"Ревенант",      SpriteType::Enemy,   1.0f,  9,  true},
	{ L"Мега бабл",     SpriteType::Enemy,   1.0f,  10, true},
	{ L"Мать",          SpriteType::Enemy,   1.0f,  11, true},
	{ L"Boss",          SpriteType::Enemy,   1.0f,  12, true},
	{ L"Петрович",      SpriteType::NPC,     1.0f,  13, true},
	{ L"Молотов",       SpriteType::NPC,     1.0f,  14, true},
	{ L"Роман",         SpriteType::NPC,     1.0f,  15, true},
	{ L"Ванька",        SpriteType::NPC,     1.0f,  16, true},
	{ L"Тихон",         SpriteType::NPC,     1.0f,  17, true},
	{ L"Виктор",        SpriteType::NPC,     1.0f,  18, true} 
};

#endif // !SPRITE
