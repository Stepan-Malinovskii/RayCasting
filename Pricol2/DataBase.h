#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "Player.h"
#include <codecvt>
#include <locale>

class Data
{
public:
	Data();
	~Data();
	PlayerDef getPlayerData();
	void savePlayerData(Player* player);

	std::vector<GunData> getGunData();
	void saveGunData(std::vector<GunData> guns);

	std::vector<int> getKeys(int key);
	std::pair<std::wstring, int> getText(int key);
private:
	std::vector<std::pair<int, std::vector<int>>>* key2key;
	std::vector<std::tuple<int, std::wstring, int>>* key2text;
};