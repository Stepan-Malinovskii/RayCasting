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

constexpr float PI = 3.14159265359f;
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
	{ L"player",        SpriteType::PlayerT, 0.3f, 5.0f,  100.0f,   -1,  false, 0,    false},
	{ L"Бабл",          SpriteType::Enemy,   1.0f, 3.0f,  70.f,      0,  true,  10,   false},
	{ L"Рогастик",      SpriteType::Enemy,   1.0f, 4.0f,  90.f,      1,  true,  15,   true},
	{ L"Розовый пинки", SpriteType::Enemy,   1.0f, 5.0f,  100.f,     2,  true,  20,   true},
	{ L"Кибер демон",   SpriteType::Enemy,   1.0f, 4.0f,  120.f,     3,  true,  25,   true},
	{ L"Спайдер",       SpriteType::Enemy,   1.0f, 5.0f,  200.f,     4,  true,  30,   true},
	{ L"Красный череп", SpriteType::Enemy,   1.0f, 6.0f,  150.f,     5,  true,  35,   false},
	{ L"Бомбастик",     SpriteType::Enemy,   1.0f, 3.0f,   300.f,    6,  true,  40,   true},
	{ L"Зеленый пинки", SpriteType::Enemy,   1.0f, 6.0f,  200.f,     7,  true,  45,   true},
	{ L"Синий череп",   SpriteType::Enemy,   1.0f, 6.0f,  200.f,     8,  true,  50,   false},
	{ L"Ревенант",      SpriteType::Enemy,   1.0f, 5.0f,  180.f,     9,  true,  55,   true},
	{ L"Мега бабл",     SpriteType::Enemy,   1.0f, 4.0f,  300.f,     10, true,  60,   false},
	{ L"Мать",          SpriteType::Enemy,   1.0f, 4.0f,  320.f,     11, true,  65,   false},
	{ L"Boss",          SpriteType::Enemy,   1.0f, 5.0f,  2000.f,    12, true,  1000, true},
	{ L"Петрович",      SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 13, true,  0,    false},
	{ L"Молотов",       SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 14, true,  0,    false},
	{ L"Роман",         SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 15, true,  0,    false},
	{ L"Ванька",        SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 16, true,  0,    false},
	{ L"Тихон",         SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 17, true,  0,    false},
	{ L"Виктор",        SpriteType::NPC,     1.0f, 1.0f,  1000000.f, 18, true,  0,    false} };
#endif // !SPRITE
