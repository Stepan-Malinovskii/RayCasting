#pragma once
#ifndef GMAG
#define GMAG

#include "Weapon.h"
#include "DataBase.h"
#include "EventSystem.h"

class WeaponManager
{
public:
	WeaponManager();
	~WeaponManager() = default;
	Gun* getGunByIndex(int index);
	Gun* getGunById(int id);
	Itemble* getItem(int id);
	std::vector<Itemble*> getTravelItem();
	std::vector<Gun*> getGuns();
	std::vector<Improve*> getImprovs();
	std::vector<Item*> getItems();
	int id = 0;
private:
	std::map<int, Itemble*> itemble;
	std::vector<std::unique_ptr<Improve>> improvements;
	std::vector<std::unique_ptr<Item>> items;
	std::vector<std::unique_ptr<Gun>> guns;
	std::vector<std::unique_ptr<Itemble>> travelItem;

	void createTravel();
	void saveGun();
	void createImprovements();
	void createGuns();
	void createItems();
	Animator<sf::Texture*> createAnimator(int gunIndex);
	Animation<sf::Texture*> createAnimation(std::vector<sf::Texture>* frames, float duration);
};

static std::vector<ItemsDef> itemsDefs{
	{ItemType::Heal, L"����� �������", 20, 1, 10, L"�������������� 20 HP"},
	{ItemType::Heal, L"������� �������", 50, 1, 20, L"�������������� 50 HP"},
	{ItemType::Heal, L"������� �������", 100, 1, 30, L"�������������� 100 HP"},
	{ItemType::MaxHeal, L"����������������� �������� 1", 20, 1, 300, L"80% - +20 � maxHP, 20% - -10 maxHP"},
	{ItemType::MaxEnergy, L"����������������� �������� 2", 2, 1, 300, L"80% - +2 � maxEnergy, 20% - -1 maxEnergy"},
	{ItemType::Armor, L"�������� ��������", 10, 50, 50, L"-10% ����������� �����, 50 �������������"},
	{ItemType::Armor, L"�������� ��������", 20, 70, 80, L"-20% ����������� �����, 70 �������������"},
	{ItemType::Armor, L"���������� ��������", 30, 90, 100, L"-30% ����������� �����, 90 �������������"},
	{ItemType::Armor, L"���������� ��������", 50, 110, 200, L"-50% ����������� �����, 110 �������������"},
	{ItemType::Patrons, L"����� ���� ��������", 50, 1, 20, L"�������� 50 ��������"},
	{ItemType::Patrons, L"������� ���� ��������", 100, 1, 35, L"�������� 100 ��������"},
	{ItemType::Patrons, L"������� ���� ��������", 150, 1, 50, L"�������� 150 ��������"}
};

static std::vector<GunDef> gunsDef{
	{10, 100, 100, 0.5f, 2.0f, 0, 0,       L"����",        L"���� - 10  | ������ - 100 �� | ���������������� - 0.5 ��� | ��������� - 1  �"},
	{10, 100, 100, 0.3f, 2.0f, 0, 0,       L"�����",       L"���� - 10  | ������ - 100 �� | ���������������� - 0.3 ��� | ��������� - 1  �"},
	{10, 20, 20, 0.4f, 20.0f, 1.5f, 0,     L"��������",    L"���� - 10  | ������ - 20  �� | ���������������� - 0.4 ��� | ��������� - 20 �"},
	{50, 5, 5, 1.0f, 5.0f, 2.0f, 340,      L"�����",       L"���� - 50  | ������ - 5   �� | ���������������� - 1.0 ��� | ��������� - 5  �"},
	{90, 2, 2, 0.6f, 5.0f, 3.0f, 500,      L"�����������", L"���� - 90  | ������ - 2   �� | ���������������� - 0.6 ��� | ��������� - 5  �"},
	{20, 30, 30, 0.4f, 15.0f, 3.0f, 600,   L"�������",     L"���� - 20  | ������ - 30  �� | ���������������� - 0.4 ��� | ��������� - 15 �"},
	{200, 1, 1, 1.0f, 10.0f, 3.0f, 700,    L"������",      L"���� - 200 | ������ - 1   �� | ���������������� - 1.0 ��� | ��������� - 10 �"},
	{10, 100, 100, 0.1f, 10.0f, 4.0f, 800, L"�������",     L"���� - 10  | ������ - 100 �� | ���������������� - 0.1 ��� | ��������� - 10 �"},
};

static std::vector<ImproveDef> improveDefs{
	{ImproveType::Magazin, L"����� �������", 1.2f, 200,   L"+20% � ������ ��������"},
	{ImproveType::Magazin, L"������� �������", 1.3f, 300, L"+30% � ������ ��������"},
	{ImproveType::Magazin, L"������� �������", 1.5f, 400, L"+50% � ������ ��������"},
	{ImproveType::Spread,  L"�������� ������", 1.4f, 150, L"-40% � ��������"},
	{ImproveType::Spread,  L"��������������� ������", 1.6f, 250, L"-60% � ��������"},
	{ImproveType::Spread,  L"���������� ������", 1.8f, 350, L"-80% � ��������"},
	{ImproveType::Damage,  L"�������������", 1.3f, 250, L"+30% � �����"},
	{ImproveType::Damage,  L"����", 1.4f, 350, L"+40% � �����"},
	{ImproveType::Damage,  L"�������", 1.5f, 450, L"+50% � �����"}
};

#endif // !GMAG

