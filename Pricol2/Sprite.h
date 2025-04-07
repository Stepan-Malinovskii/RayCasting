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

enum SpriteType
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
	float speed;
	float maxHealpoint;
	int texture;
	bool isDirectional;
	int midleDrop;
	bool isCanRun;
	float atackDist;
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

	void move(Map* map, sf::Vector2f move);
	void takeDamage(float damage);
	void update(float deltaTime);
	int getTextIndex();
	void changeState(SpriteState state);
	int id;
	SpriteDef spDef;
	MapSprite spMap;

	sf::Texture* texture;
	
	SpriteState state;
	bool isDamages;
	int textSize;
	std::set<std::tuple<int, int>> blockmap_coords;

private:
	float timeAtecked;
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

static std::vector<SpriteDef> spriteDefs = {
	{ L"player",        SpriteType::PlayerT, 0.3f, 5.0f,  100.0f,   -1,  false, 0,    false, 0},
	{ L"Бабл",          SpriteType::Enemy,   1.0f, 3.0f,  70.f,      0,  true,  10,   true,  5},
	{ L"Синчик",        SpriteType::Enemy,   1.0f, 4.0f,  90.f,      1,  true,  15,   true,  5},
	{ L"Розовый пинки", SpriteType::Enemy,   1.0f, 5.0f,  100.f,     2,  true,  20,   true,  5},
	{ L"Кибер демон",   SpriteType::Enemy,   1.0f, 4.0f,  120.f,     3,  true,  25,   true,  20},
	{ L"Спайдер",       SpriteType::Enemy,   1.0f, 5.0f,  200.f,     4,  true,  30,   true,  20},
	{ L"Красный череп", SpriteType::Enemy,   1.0f, 6.0f,  150.f,     5,  true,  35,   true,  5},
	{ L"Бомбастик",     SpriteType::Enemy,   1.0f, 3.0f,   300.f,    6,  true,  40,   true,  20},
	{ L"Зеленый пинки", SpriteType::Enemy,   1.0f, 6.0f,  200.f,     7,  true,  45,   true,  5},
	{ L"Розовый череп", SpriteType::Enemy,   1.0f, 6.0f,  200.f,     8,  true,  50,   true,  5},
	{ L"Ревенант",      SpriteType::Enemy,   1.0f, 5.0f,  180.f,     9,  true,  55,   true,  20},
	{ L"Мега бабл",     SpriteType::Enemy,   1.0f, 4.0f,  300.f,     10, true,  60,   false, 20},
	{ L"Мать",          SpriteType::Enemy,   1.0f, 4.0f,  320.f,     11, true,  65,   true,  5},
	{ L"Boss",          SpriteType::Enemy,   1.0f, 5.0f,  2000.f,    12, true,  1000, true,  5},
	{ L"Петрович",      SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 13, true,  0,    false, 0},
	{ L"Молотов",       SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 14, true,  0,    false, 0},
	{ L"Роман",         SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 15, true,  0,    false, 0},
	{ L"Ванька",        SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 16, true,  0,    false, 0},
	{ L"Тихон",         SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 17, true,  0,    false, 0},
	{ L"Виктор",        SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 18, true,  0,    false, 0} };
#endif // !SPRITE
