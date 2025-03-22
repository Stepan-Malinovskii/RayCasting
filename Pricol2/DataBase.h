#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "Player.h"
#include <codecvt>
#include <locale>

struct PlayerDef
{
	int maxHp;
	int nowHp;
};

struct GunDef
{
	int maxCount;
	int nowCount;
	int damage;
	float maxDist;
};

class Data
{
public:
	Data();
	~Data();
	PlayerDef getPlayerData();
	void savePlayerData(Player* player);

	std::vector<GunDef> getGunData();
	void saveGunData(std::vector<Gun*> guns);

	std::vector<int> getKeys(int key);
	std::pair<std::wstring, int> getText(int key);
private:
	std::vector<std::pair<int, std::vector<int>>>* key2key;
	std::vector<std::tuple<int, std::wstring, int>>* key2text;
};