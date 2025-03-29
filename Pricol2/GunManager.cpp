#include "GunManager.h"

WeaponManager::WeaponManager(Data* _data)
{
	data = _data;

	for (int i = 0; i < improveDefs.size(); i++)
	{
		improvements.push_back(std::make_unique<Improve>(improveDefs[i]));
	}

	for (int i = 0; i < itemsDefs.size(); i++)
	{
		items.push_back(std::make_unique<Item>(itemsDefs[i]));
	}

	auto gunsData = data->getGunData();

	for (int i = 0; i < gunsDef.size(); i++)
	{
		Animation<sf::Texture*> shutAnim{};
		int cnt = Resources::gunsFireAnim[i].size();
		for (int j = 0; j < cnt; j++)
		{
			shutAnim.setKeyframe((j + 1) / (float)cnt * gunsDef[i].shutTime, &Resources::gunsFireAnim[i][j]);
		}
		if (cnt != 0) shutAnim.setKeyframe(gunsDef[i].shutTime, &Resources::gunsFireAnim[i][cnt - 1]);

		Animation<sf::Texture*> resetAnim{};
		cnt = Resources::gunsResetAnim[i].size();
		for (int j = 0; j < cnt; j++)
		{
			resetAnim.setKeyframe((j + 1) / (float)cnt * gunsDef[i].resetTime, &Resources::gunsResetAnim[i][j]);
		}
		if (cnt != 0) resetAnim.setKeyframe(gunsDef[i].resetTime, &Resources::gunsResetAnim[i][cnt - 1]);

		Animator<sf::Texture*> animr{ &Resources::gunsBaseText[i], {shutAnim, resetAnim} };

		auto def = gunsDef[i];
		def.nowCount = gunsData[i].nowCount;

		guns.push_back(std::make_unique<Gun>(def, i > 1));
		guns.back()->setAnimator(animr);
		guns.back()->setSound(&Resources::gunsShutSound[i],
			&Resources::gunsResetSound[i],
			&Resources::gunCantShoutSound);

		for (auto im : gunsData[i].improveId)
		{
			guns.back()->trySetImprove(improvements[im].get());
		}
	}
}

WeaponManager::~WeaponManager() {}

Gun* WeaponManager::getGun(int index) { return guns[index].get(); }

Item* WeaponManager::getItem(int index) { return items[index].get(); }

std::vector<Gun*> WeaponManager::getGuns()
{
	std::vector<Gun*> g;
	for (int i = 0; i < guns.size(); i++)
	{
		g.push_back(guns[i].get());
	}
	return g;
}

std::vector<Improve*> WeaponManager::getImprovs()
{
	std::vector<Improve*> g;
	for (int i = 0; i < improvements.size(); i++)
	{
		g.push_back(improvements[i].get());
	}
	return g;
}

std::vector<Item*> WeaponManager::getItems()
{
	std::vector<Item*> g;
	for (int i = 0; i < items.size(); i++)
	{
		g.push_back(items[i].get());
	}
	return g;
}

void WeaponManager::saveGun()
{
	std::vector<GunData> defs;
	for (int i = 0; i < guns.size(); i++)
	{
		defs.push_back(guns[i]->getGunData());
	}

	data->saveGunData(defs);
}
