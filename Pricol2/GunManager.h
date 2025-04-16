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
	{ItemType::Heal, L"Малая аптечка", 20, 1, 10, L"Востанавливает 20 HP"},
	{ItemType::Heal, L"Средняя аптечка", 50, 1, 20, L"Востанавливает 50 HP"},
	{ItemType::Heal, L"Большая аптечка", 100, 1, 30, L"Востанавливает 100 HP"},
	{ItemType::MaxHeal, L"Эксперементальное снадобье 1", 20, 1, 300, L"80% - +20 к maxHP, 20% - -10 maxHP"},
	{ItemType::MaxEnergy, L"Эксперементальное снадобье 2", 2, 1, 300, L"80% - +2 к maxEnergy, 20% - -1 maxEnergy"},
	{ItemType::Armor, L"Железная пластина", 10, 50, 50, L"-10% получаемого урона, 50 использований"},
	{ItemType::Armor, L"Стальная пластина", 20, 70, 80, L"-20% получаемого урона, 70 использований"},
	{ItemType::Armor, L"Кевларовая пластина", 30, 90, 100, L"-30% получаемого урона, 90 использований"},
	{ItemType::Armor, L"Карбоновая пластина", 50, 110, 200, L"-50% получаемого урона, 110 использований"},
	{ItemType::Patrons, L"Малый ящик патронов", 50, 1, 20, L"Содержит 50 патронов"},
	{ItemType::Patrons, L"Средний ящик патронов", 100, 1, 35, L"Содержит 100 патронов"},
	{ItemType::Patrons, L"Большой ящик патронов", 150, 1, 50, L"Содержит 150 патронов"}
};

static std::vector<GunDef> gunsDef{
	{10, 100, 100, 0.5f, 2.0f, 0, 0,       L"Нога",        L"Урон - 10  | Обойма - 100 пт | Скорострельность - 0.5 сек | Дистанция - 1  м"},
	{10, 100, 100, 0.3f, 2.0f, 0, 0,       L"Кулак",       L"Урон - 10  | Обойма - 100 пт | Скорострельность - 0.3 сек | Дистанция - 1  м"},
	{10, 20, 20, 0.4f, 20.0f, 1.5f, 0,     L"Пистолет",    L"Урон - 10  | Обойма - 20  пт | Скорострельность - 0.4 сек | Дистанция - 20 м"},
	{50, 5, 5, 1.0f, 5.0f, 2.0f, 340,      L"Обрез",       L"Урон - 50  | Обойма - 5   пт | Скорострельность - 1.0 сек | Дистанция - 5  м"},
	{90, 2, 2, 0.6f, 5.0f, 3.0f, 500,      L"Двухстволка", L"Урон - 90  | Обойма - 2   пт | Скорострельность - 0.6 сек | Дистанция - 5  м"},
	{20, 30, 30, 0.4f, 15.0f, 3.0f, 600,   L"Автомат",     L"Урон - 20  | Обойма - 30  пт | Скорострельность - 0.4 сек | Дистанция - 15 м"},
	{200, 1, 1, 1.0f, 10.0f, 3.0f, 700,    L"Базука",      L"Урон - 200 | Обойма - 1   пт | Скорострельность - 1.0 сек | Дистанция - 10 м"},
	{10, 100, 100, 0.1f, 10.0f, 4.0f, 800, L"Миниган",     L"Урон - 10  | Обойма - 100 пт | Скорострельность - 0.1 сек | Дистанция - 10 м"},
};

static std::vector<ImproveDef> improveDefs{
	{ImproveType::Magazin, L"Малый магазин", 1.2f, 200,   L"+20% к объему магазина"},
	{ImproveType::Magazin, L"Средний магазин", 1.3f, 300, L"+30% к объему магазина"},
	{ImproveType::Magazin, L"Большой магазин", 1.5f, 400, L"+50% к объему магазина"},
	{ImproveType::Spread,  L"Лазерный прицел", 1.4f, 150, L"-40% к разбросу"},
	{ImproveType::Spread,  L"Голографический прицел", 1.6f, 250, L"-60% к разбросу"},
	{ImproveType::Spread,  L"Оптический прицел", 1.8f, 350, L"-80% к разбросу"},
	{ImproveType::Damage,  L"Пламягаситель", 1.3f, 250, L"+30% к урону"},
	{ImproveType::Damage,  L"Упор", 1.4f, 350, L"+40% к урону"},
	{ImproveType::Damage,  L"Приклад", 1.5f, 450, L"+50% к урону"}
};

#endif // !GMAG

