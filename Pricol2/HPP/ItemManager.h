#pragma once
#ifndef GMAG
#define GMAG

#include "Weapon.h"
#include "DataBase.h"
#include "EventSystem.h"
#include "MapManager.h"

class ItemManager
{
public:
	ItemManager();
	~ItemManager() = default;
	Gun* getGunByIndex(int index);
	Gun* getGunById(int id);
	Itemble* getItemble(int id);
private:
	std::map<int, Itemble*> itemble;
	std::vector<std::unique_ptr<Improve>> improvements;
	std::vector<std::unique_ptr<Item>> items;
	std::vector<std::unique_ptr<Gun>> guns;
	std::vector<std::unique_ptr<Item>> travelItem;

	void resetGuns();
	void saveGun();
	void createImprovements();
	void createGuns();
	void createItems();
	void createTraveler();
	Animator<sf::Texture*> createAnimator(int gunIndex);
	Animation<sf::Texture*> createAnimation(std::vector<sf::Texture>* frames, float duration);
};

static std::vector<ItemsDef> itemsDefs{
	{9,  ItemType::Heal, L"����� �������", 20, 1, 10, L"�������������� 20 HP"},
	{10, ItemType::Heal, L"������� �������", 50, 1, 20, L"�������������� 50 HP"},
	{11, ItemType::Heal, L"������� �������", 100, 1, 30, L"�������������� 100 HP"},
	{12, ItemType::MaxHeal, L"����������������� �������� 1", 20, 1, 300, L"80% - +20 � maxHP, 20% - -10 maxHP"},
	{13, ItemType::MaxEnergy, L"����������������� �������� 2", 2, 1, 300, L"80% - +2 � maxEnergy, 20% - -1 maxEnergy"},
	{14, ItemType::Armor, L"�������� ��������", 10, 50, 50, L"-10% ����������� �����, 50 �������������"},
	{15, ItemType::Armor, L"�������� ��������", 20, 70, 80, L"-20% ����������� �����, 70 �������������"},
	{16, ItemType::Armor, L"���������� ��������", 30, 90, 100, L"-30% ����������� �����, 90 �������������"},
	{17, ItemType::Armor, L"���������� ��������", 50, 110, 200, L"-50% ����������� �����, 110 �������������"},
	{18, ItemType::Patrons, L"����� ���� ��������", 50, 1, 20, L"�������� 50 ��������"},
	{19, ItemType::Patrons, L"������� ���� ��������", 100, 1, 35, L"�������� 100 ��������"},
	{20, ItemType::Patrons, L"������� ���� ��������", 150, 1, 50, L"�������� 150 ��������"}
};

static std::vector<GunDef> gunsDef{
	{21, 10, 100, 100, 0.5f, 2.0f, 0, 0,       L"����",        L"���� - 10  | ������ - 100 �� | ���������������� - 0.5 ��� | ��������� - 1  �"},
	{22, 10, 100, 100, 0.3f, 2.0f, 0, 0,       L"�����",       L"���� - 10  | ������ - 100 �� | ���������������� - 0.3 ��� | ��������� - 1  �"},
	{23, 10, 20, 20, 0.4f, 20.0f, 1.5f, 0,     L"��������",    L"���� - 10  | ������ - 20  �� | ���������������� - 0.4 ��� | ��������� - 20 �"},
	{24, 50, 5, 5, 1.0f, 5.0f, 2.0f, 340,      L"�����",       L"���� - 50  | ������ - 5   �� | ���������������� - 1.0 ��� | ��������� - 5  �"},
	{25, 90, 2, 2, 0.6f, 5.0f, 3.0f, 500,      L"�����������", L"���� - 90  | ������ - 2   �� | ���������������� - 0.6 ��� | ��������� - 5  �"},
	{26, 20, 30, 30, 0.4f, 15.0f, 3.0f, 600,   L"�������",     L"���� - 20  | ������ - 30  �� | ���������������� - 0.4 ��� | ��������� - 15 �"},
	{27, 200, 1, 1, 1.0f, 10.0f, 3.0f, 700,    L"������",      L"���� - 200 | ������ - 1   �� | ���������������� - 1.0 ��� | ��������� - 10 �"},
	{28, 10, 100, 100, 0.1f, 10.0f, 4.0f, 800, L"�������",     L"���� - 10  | ������ - 100 �� | ���������������� - 0.1 ��� | ��������� - 10 �"},
};

static std::vector<ImproveDef> improveDefs{
	{0, ImproveType::Magazin, L"����� �������", 1.2f, 200,   L"+20% � ������ ��������"},
	{1, ImproveType::Magazin, L"������� �������", 1.3f, 300, L"+30% � ������ ��������"},
	{2, ImproveType::Magazin, L"������� �������", 1.5f, 400, L"+50% � ������ ��������"},
	{3, ImproveType::Spread,  L"�������� ������", 1.4f, 150, L"-40% � ��������"},
	{4, ImproveType::Spread,  L"��������������� ������", 1.6f, 250, L"-60% � ��������"},
	{5, ImproveType::Spread,  L"���������� ������", 1.8f, 350, L"-80% � ��������"},
	{6, ImproveType::Damage,  L"�������������", 1.3f, 250, L"+30% � �����"},
	{7, ImproveType::Damage,  L"����", 1.4f, 350, L"+40% � �����"},
	{8, ImproveType::Damage,  L"�������", 1.5f, 450, L"+50% � �����"}
};

static std::vector<ItemsDef> travelerDefs{
	{29, ItemType::Heal, L"", NEXT_LEVEL_N, 1, 50,  L"����������� �� ��������� �������"},
	{30, ItemType::Heal, L"", ARENA_1_N,    1, 100, L"����������� �� ��������������� 1"},
	{31, ItemType::Heal, L"", ARENA_2_N,    1, 150, L"����������� �� ��������������� 2"},
	{32, ItemType::Heal, L"", ARENA_3_N,    1, 200, L"����������� �� ��������������� 3"},
	{33, ItemType::Heal, L"", BOSS_N,       1, 250, L"������� � �����"}
};

#endif // !GMAG

