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

struct GameStateData
{
	bool isFirstGame;
	int effectVolume;
	int soundVolume;
	int levelNumber;
	bool isLevelBase;
};

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

	GameStateData getGameState();
	void saveGameState(GameStateData data);

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

class GameState
{
public:
	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;

	static GameState& getInstance()
	{
		static GameState instanceGame;
		return instanceGame;
	}

	GameStateData data;
private:
	GameState()
	{
		auto& dataBase = Data::getInstance();
		data = dataBase.getGameState();
	}
	~GameState()
	{
		auto& dataBase = Data::getInstance();
		dataBase.saveGameState(data);
	}
};

#endif // !DATA