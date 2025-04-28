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

	void saveGun();
	void createImprovements();
	void createGuns();
	void createItems();
	void createTraveler();
	Animator<sf::Texture*> createAnimator(int gunIndex);
	Animation<sf::Texture*> createAnimation(std::vector<sf::Texture>* frames, float duration);
};

static std::vector<ItemsDef> itemsDefs{
	{9,  ItemType::Heal, L"Малая аптечка", 20, 1, 10, L"Востанавливает 20 HP"},
	{10, ItemType::Heal, L"Средняя аптечка", 50, 1, 20, L"Востанавливает 50 HP"},
	{11, ItemType::Heal, L"Большая аптечка", 100, 1, 30, L"Востанавливает 100 HP"},
	{12, ItemType::MaxHeal, L"Эксперементальное снадобье 1", 20, 1, 300, L"80% - +20 к maxHP, 20% - -10 maxHP"},
	{13, ItemType::MaxEnergy, L"Эксперементальное снадобье 2", 2, 1, 300, L"80% - +2 к maxEnergy, 20% - -1 maxEnergy"},
	{14, ItemType::Armor, L"Железная пластина", 10, 50, 50, L"-10% получаемого урона, 50 использований"},
	{15, ItemType::Armor, L"Стальная пластина", 20, 70, 80, L"-20% получаемого урона, 70 использований"},
	{16, ItemType::Armor, L"Кевларовая пластина", 30, 90, 100, L"-30% получаемого урона, 90 использований"},
	{17, ItemType::Armor, L"Карбоновая пластина", 50, 110, 200, L"-50% получаемого урона, 110 использований"},
	{18, ItemType::Patrons, L"Малый ящик патронов", 50, 1, 20, L"Содержит 50 патронов"},
	{19, ItemType::Patrons, L"Средний ящик патронов", 100, 1, 35, L"Содержит 100 патронов"},
	{20, ItemType::Patrons, L"Большой ящик патронов", 150, 1, 50, L"Содержит 150 патронов"}
};

static std::vector<GunDef> gunsDef{
	{21, 10, 100, 100, 0.5f, 2.0f, 0, 0,       L"Нога",        L"Урон - 10  | Обойма - 100 пт | Скорострельность - 0.5 сек | Дистанция - 1  м"},
	{22, 10, 100, 100, 0.3f, 2.0f, 0, 0,       L"Кулак",       L"Урон - 10  | Обойма - 100 пт | Скорострельность - 0.3 сек | Дистанция - 1  м"},
	{23, 10, 20, 20, 0.4f, 20.0f, 1.5f, 0,     L"Пистолет",    L"Урон - 10  | Обойма - 20  пт | Скорострельность - 0.4 сек | Дистанция - 20 м"},
	{24, 50, 5, 5, 1.0f, 5.0f, 2.0f, 340,      L"Обрез",       L"Урон - 50  | Обойма - 5   пт | Скорострельность - 1.0 сек | Дистанция - 5  м"},
	{25, 90, 2, 2, 0.6f, 5.0f, 3.0f, 500,      L"Двухстволка", L"Урон - 90  | Обойма - 2   пт | Скорострельность - 0.6 сек | Дистанция - 5  м"},
	{26, 20, 30, 30, 0.4f, 15.0f, 3.0f, 600,   L"Автомат",     L"Урон - 20  | Обойма - 30  пт | Скорострельность - 0.4 сек | Дистанция - 15 м"},
	{27, 200, 1, 1, 1.0f, 10.0f, 3.0f, 700,    L"Базука",      L"Урон - 200 | Обойма - 1   пт | Скорострельность - 1.0 сек | Дистанция - 10 м"},
	{28, 10, 100, 100, 0.1f, 10.0f, 4.0f, 800, L"Миниган",     L"Урон - 10  | Обойма - 100 пт | Скорострельность - 0.1 сек | Дистанция - 10 м"},
};

static std::vector<ImproveDef> improveDefs{
	{0, ImproveType::Magazin, L"Малый магазин", 1.2f, 200,   L"+20% к объему магазина"},
	{1, ImproveType::Magazin, L"Средний магазин", 1.3f, 300, L"+30% к объему магазина"},
	{2, ImproveType::Magazin, L"Большой магазин", 1.5f, 400, L"+50% к объему магазина"},
	{3, ImproveType::Spread,  L"Лазерный прицел", 1.4f, 150, L"-40% к разбросу"},
	{4, ImproveType::Spread,  L"Голографический прицел", 1.6f, 250, L"-60% к разбросу"},
	{5, ImproveType::Spread,  L"Оптический прицел", 1.8f, 350, L"-80% к разбросу"},
	{6, ImproveType::Damage,  L"Пламягаситель", 1.3f, 250, L"+30% к урону"},
	{7, ImproveType::Damage,  L"Упор", 1.4f, 350, L"+40% к урону"},
	{8, ImproveType::Damage,  L"Приклад", 1.5f, 450, L"+50% к урону"}
};

static std::vector<ItemsDef> travelerDefs{
	{29, ItemType::Heal, L"", NEXT_LEVEL_N, 1, 50,  L"Перемещение на следующий уровень"},
	{30, ItemType::Heal, L"", ARENA_1_N,    1, 100, L"Перемещение на преобразоваетль 1"},
	{31, ItemType::Heal, L"", ARENA_2_N,    1, 150, L"Перемещение на преобразоваетль 2"},
	{32, ItemType::Heal, L"", ARENA_3_N,    1, 200, L"Перемещение на преобразоваетль 3"},
	{33, ItemType::Heal, L"", BOSS_N,       1, 250, L"Поездка к боссу"}
};

#endif // !GMAG

