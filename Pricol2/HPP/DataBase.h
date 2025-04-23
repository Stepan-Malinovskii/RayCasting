#pragma once
#ifndef DATA
#define DATA

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
	Data(const Data&) = delete;
	Data& operator=(const Data&) = delete;

	static Data& getInstance()
	{
		static Data instance;
		return instance;
	}

	PlayerDef getPlayerData();
	void savePlayerData(Player* player);

	std::vector<GunData> getGunData();
	void saveGunData(std::vector<GunData> guns);

	std::vector<std::pair<int, int>> getInvent();
	void saveInvent(std::vector<std::pair<int, int>> inv);

	std::vector<int> getKeys(int key);
	std::pair<std::wstring, int> getText(int key);
private:
	Data() = default;
	~Data() = default;

	std::vector<std::pair<int, std::vector<int>>> loadKeyData();
	std::vector<std::tuple<int, std::wstring, int>> loadTextData();
};

#endif // !DATA