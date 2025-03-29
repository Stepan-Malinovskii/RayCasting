#pragma once
#ifndef GMAG
#define GMAG

#include "Weapon.h"
#include "DataBase.h"

class WeaponManager
{
public:
	WeaponManager(Data* data);
	~WeaponManager();
	Gun* getGun(int index);
	Item* getItem(int index);
	std::vector<Gun*> getGuns();
	std::vector<Improve*> getImprovs();
	std::vector<Item*> getItems();
	void saveGun();
private:
	std::vector<std::unique_ptr<Improve>> improvements;
	std::vector<std::unique_ptr<Item>> items;
	std::vector<std::unique_ptr<Gun>> guns;
	Data* data;
};

static std::vector<ItemsDef> itemsDefs{
	{0, ItemType::Heal, L"Большая аптечка", 100, 1, 30, L"Востанавливает 100 HP"},
	{1, ItemType::Heal, L"Средняя аптечка", 50, 1, 20, L"Востанавливает 50 HP"},
	{2, ItemType::Heal, L"Малая аптечка", 20, 1, 10, L"Востанавливает 20 HP"},
	{3, ItemType::MaxHeal, L"Эксперементальное снадобье 1", 20, 1, 300, L"80% - +20 к maxHP, 20% - -10 maxHP"},
	{4, ItemType::MaxEnergy, L"Эксперементальное снадобье 2", 20, 1, 300, L"80% - +20 к maxEnergy, 20% - -10 maxEnergy"},
	{5, ItemType::Armor, L"Железная пластина", 10, 50, 50, L"-10% получаемого урона, 50 использований"},
	{6, ItemType::Armor, L"Стальная пластина", 20, 70, 80, L"-20% получаемого урона, 70 использований"},
	{7, ItemType::Armor, L"Кевларовая пластина", 30, 90, 100, L"-30% получаемого урона, 90 использований"},
	{8, ItemType::Armor, L"Карбоновая пластина", 50, 110, 200, L"-50% получаемого урона, 110 использований"}
};

static std::vector<GunDef> gunsDef{
	{0, 10, 100, 100, 0.5f, 1.0f, 0, 0, L"Кулаки", L"Урон - 10; Обойма - 100 пт; Скорострельность - 0.5f сек; Дистанция - 1.0f м."},
	{1, 10, 100, 100, 0.3f, 1.0f, 0, 0, L"Нога", L"Урон - 10; Обойма - 100 пт; Скорострельность - 0.3f сек; Дистанция - 1.0f м."},
	{2, 10, 20, 20, 0.4f, 20.0f, 1.5f, 0, L"Пистолет", L"Урон - 10; Обойма - 20 пт; Скорострельность - 0.4f сек; Дистанция - 20.0f м."},
	{3, 50, 5, 5, 1.0f, 5.0f, 2.0f, 340, L"Обрез", L"Урон - 50; Обойма - 5 пт; Скорострельность - 1.0f сек; Дистанция - 5.0f м."},
	{4, 90, 2, 2, 0.6f, 5.0f, 3.0f, 500, L"Двухстволка", L"Урон - 90; Обойма - 2 пт; Скорострельность - 0.6f сек; Дистанция - 5.0f м."},
	{5, 20, 30, 30, 0.4f, 15.0f, 3.0f, 600, L"Автомат", L"Урон - 20; Обойма - 30 пт; Скорострельность - 0.4f сек; Дистанция - 15.0f м."},
	{6, 200, 1, 1, 1.0f, 10.0f, 3.0f, 700, L"Базука", L"Урон - 200; Обойма - 1 пт; Скорострельность - 1.0f сек; Дистанция - 10.0f м."},
	{7, 10, 100, 100, 0.1f, 10.0f, 4.0f, 800, L"Миниган", L"Урон - 10; Обойма - 100 пт; Скорострельность - 0.1f сек; Дистанция - 10.0f м."},
};

static std::vector<ImproveDef> improveDefs{
	{0, ImproveType::Magazin, L"Малый магазин", 1.2f, 200, L"+20% к объему магазина"},
	{1, ImproveType::Magazin, L"Средний магазин", 1.3f, 300, L"+30% к объему магазина"},
	{2, ImproveType::Magazin, L"Большой магазин", 1.4f, 400, L"+40% к объему магазина"},
	{3, ImproveType::Spread, L"Лазерный прицел", 1.3f, 150, L"-30% к разбросу"},
	{4, ImproveType::Spread, L"Голографический  прицел", 1.45f, 250, L"-45% к разбросу"},
	{5, ImproveType::Spread, L"Оптический прицел", 1.6f, 350, L"-60% к разбросу"},
	{6, ImproveType::Damage, L"Пламя гаситель", 1.3f, 250, L"+30% к урону"},
	{7, ImproveType::Damage, L"Упор", 1.4f, 350, L"+40% к урону"},
	{8, ImproveType::Damage, L"Приклад", 1.5f, 450, L"+50% к урону"}
};

#endif // !GMAG

