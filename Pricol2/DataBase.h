#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <codecvt>
#include <locale>

class Data
{
public:
	Data();
	std::vector<int> getKeys(int key);
	std::pair<std::wstring, int> getText(int key);
private:
	std::vector<std::pair<int, std::vector<int>>>* key2key;
	std::vector<std::tuple<int, std::wstring, int>>* key2text;
};