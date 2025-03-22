#include "DataBase.h"
#pragma warning(disable : 4996)
Data::Data()
{
	key2key = new std::vector<std::pair<int, std::vector<int>>>();
	key2text = new std::vector<std::tuple<int, std::wstring, int>>();

	std::wstring line;
	std::wifstream fileIn("KeyData.txt");
	fileIn.imbue(std::locale(fileIn.getloc(), new std::codecvt_utf8<wchar_t>));
	if (fileIn.is_open())
	{
		int fkey;
		while (std::getline(fileIn, line))
		{
			int st = line.find(L'[');
			int end = line.find(L']');
			fkey = std::stoi(line.substr(st + 1, end - st - 1));
			line = line.substr(end + 3, line.size() - end - 4);
			std::vector<int> keys;
			while (line.size() != 0)
			{
				int j = line.find(L',');
				keys.push_back(std::stoi(line.substr(0, j)));
				line = line.substr(j + 1);
			}
			key2key->push_back({ fkey, keys });
		}
	}
	fileIn.close();

	std::wifstream fileIn1("TextData.txt");
	fileIn1.imbue(std::locale(fileIn1.getloc(), new std::codecvt_utf8<wchar_t>));
	if (fileIn1.is_open())
	{
		int fkey, eKey;
		std::wstring text;
		while (std::getline(fileIn1, line))
		{
			int st = line.find(L'[');
			int end = line.find(L']');
			fkey = std::stoi(line.substr(st + 1, end - st - 1));
			st = line.rfind(L'[');
			
			text = line.substr(end + 2, st - end - 3);
			end = line.rfind(L']');
			eKey = std::stoi(line.substr(st + 1, end - st - 1));
			key2text->push_back({ fkey, text, eKey });
		}
	}
	fileIn.close();
}

Data::~Data()
{
	delete key2key;
	delete key2text;
}

std::vector<int> Data::getKeys(int key)
{
	for (int i = 0; i < key2key->size(); i++)
	{
		if (key == (*key2key)[i].first) {
			return (*key2key)[i].second;
		}
	}
}

std::pair<std::wstring, int> Data::getText(int key)
{
	for (int i = 0; i < key2text->size(); i++)
	{
		if (key == std::get<0>((*key2text)[i]))
		{
			return { std::get<1>((*key2text)[i]) ,std::get<2>((*key2text)[i]) };
		}
	}
}

PlayerDef Data::getPlayerData()
{
	std::ifstream in{ "Texture/playerData.plr", std::ios::in | std::ios::binary };
	if (!in.is_open()) return{-1, 0};
	PlayerDef plDef{};
	in.read(reinterpret_cast<char*>(&plDef), sizeof(plDef));
	in.close();
	return plDef;
}

void Data::savePlayerData(Player* player)
{
	std::ofstream out{ "Texture/playerData.plr", std::ios::out | std::ios::binary };
	if (!out.is_open()) return;

	PlayerDef pldef{ player->sprite->spDef.maxHealpoint,
		player->sprite->spMap.nowHealPoint };

	out.write(reinterpret_cast<const char*>(&pldef), sizeof(PlayerDef));
	out.close();
}

std::vector<GunDef> Data::getGunData()
{

}

void Data::saveGunData(std::vector<Gun*> guns)
{

}