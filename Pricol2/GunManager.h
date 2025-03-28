#ifndef GMAG
#include "Weapon.h"
#include "DataBase.h"

class WeaponManager
{
public:
	WeaponManager(Data* data);
	~WeaponManager();
	Gun* getGun(int index);
	Item* getItem(int index);
	void saveGun();
private:
	std::vector<std::unique_ptr<Improve>> improvements;
	std::vector<std::unique_ptr<Item>> items;
	std::vector<std::unique_ptr<Gun>> guns;
	Data* data;
};

static std::vector<ItemsDef> itemsDefs{
	{0, ItemType::Heal, L"������� �������", 100, 1, 30},
	{1, ItemType::Heal, L"������� �������", 50, 1, 20},
	{2, ItemType::Heal, L"����� �������", 20, 1, 10},
	{3, ItemType::MaxHeal, L"����������������� �������� 1", 20, 1, 300},
	{4, ItemType::MaxEnergy, L"����������������� �������� 2", 20, 1, 300},
	{5, ItemType::Armor, L"�������� ��������", 10, 50, 50},
	{6, ItemType::Armor, L"�������� ��������", 20, 70, 80},
	{7, ItemType::Armor, L"���������� ��������", 30, 90, 100},
	{8, ItemType::Armor, L"���������� ��������", 50, 110, 200}
};

static std::vector<GunDef> gunsDef {
	{10, 100, 100, 0.5f, 1.0f, 0, 0},
	{10, 100, 100, 0.3f, 1.0f, 0, 0},
	{10, 20, 20, 0.4f, 20.0f, 1.5f, 0},
	{50, 5, 5, 1.0f, 5.0f, 2.0f, 340},
	{90, 2, 2, 0.6f, 5.0f, 3.0f, 500},
	{20, 30, 30, 0.4f, 15.0f, 3.0f, 600},
	{200, 1, 1, 1.0f, 10.0f, 3.0f, 700},
	{10, 100, 100, 0.1f, 10.0f, 4.0f, 800},
};

static std::vector<ImproveDef> improveDefs {
	{0, ImproveType::Magazin, L"����� �������", 1.2f, 200},
	{1, ImproveType::Magazin, L"������� �������", 1.3f, 300},
	{2, ImproveType::Magazin, L"������� �������", 1.4f, 400},
	{3, ImproveType::Spread, L"�������� ������", 1.3f, 150},
	{4, ImproveType::Spread, L"���������������  ������", 1.45f, 250},
	{5, ImproveType::Spread, L"���������� ������", 1.6f, 350},
	{6, ImproveType::Damage, L"����� ��������", 1.3f, 250},
	{7, ImproveType::Damage, L"����", 1.4f, 350},
	{8, ImproveType::Damage, L"�������", 1.5f, 450}
};

#endif // !GMAG

