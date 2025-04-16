#include "GunManager.h"

WeaponManager::WeaponManager() : id{ 0 }
{
	createImprovements();
	createItems();
	createGuns();
	auto& event = EventSystem::getInstance();
	event.subscribe<int>("SAVE", [=](const int& NON) {saveGun();});
}

void WeaponManager::createImprovements()
{
	for (int i = 0; i < improveDefs.size(); i++)
	{
		improvements.push_back(std::make_unique<Improve>(improveDefs[i], id));
		itemble[id] = improvements.back().get();
		id++;
	}
}

void  WeaponManager::createItems()
{
	for (int i = 0; i < itemsDefs.size(); i++)
	{
		items.push_back(std::make_unique<Item>(itemsDefs[i], id));
		itemble[id] = items.back().get();
		id++;
	}
}

void  WeaponManager::createGuns()
{
	auto& data = Data::getInstance();
	auto gunsData = data.getGunData();

	for (int i = 0; i < gunsDef.size(); i++)
	{
		GunDef def = gunsDef[i];
		def.nowCount = gunsData[i].nowCount;

		guns.push_back(std::make_unique<Gun>(def, i > 1, id, i));
		guns.back()->setAnimator(std::move(createAnimator(i)));

		itemble[id] = guns.back().get();

		for (auto im : gunsData[i].improveId)
		{
			guns.back()->trySetImprove(improvements[im].get());
		}

		id++;
	}
}

Animator<sf::Texture*> WeaponManager::createAnimator(int gunIndex)
{
	return Animator<sf::Texture*>(&Resources::gunsBaseText[gunIndex], 
		{ createAnimation(&Resources::gunsFireAnim[gunIndex], gunsDef[gunIndex].shutTime)
		, createAnimation(&Resources::gunsResetAnim[gunIndex], gunsDef[gunIndex].resetTime) });
}

Animation<sf::Texture*> WeaponManager::createAnimation(std::vector<sf::Texture>* frames, float duration)
{
	Animation<sf::Texture*> anim;
	int count = frames->size();

	for (int j = 0; j < count; ++j)
	{
		anim.setKeyframe((j + 1) / (float)count * duration, &(*frames)[j]);
	}

	if (count > 0)
	{
		anim.setKeyframe(duration, &(*frames)[count - 1]);
	}

	return anim;
}

Gun* WeaponManager::getGunByIndex(int index) { return index >= 0 && index < guns.size() ? 
	guns[index].get(): throw "Invalid index"; }

Gun* WeaponManager::getGunById(int id) { return dynamic_cast<Gun*>(itemble[id]); }

Itemble* WeaponManager::getItem(int index) { return itemble[index]; }

std::vector<Gun*> WeaponManager::getGuns()
{
	std::vector<Gun*> result;
	result.reserve(guns.size());

	for (const auto& gun : guns)
	{
		result.push_back(gun.get());
	}

	return result;
}

std::vector<Improve*> WeaponManager::getImprovs()
{
	std::vector<Improve*> result;
	result.reserve(improvements.size());

	for (const auto& improve : improvements)
	{
		result.push_back(improve.get());
	}

	return result;
}

std::vector<Item*> WeaponManager::getItems()
{
	std::vector<Item*> result;
	result.reserve(items.size());

	for (const auto& item : items)
	{
		result.push_back(item.get());
	}

	return result;
}

void WeaponManager::saveGun()
{
	std::vector<GunData> defs;
	defs.reserve(guns.size());

	for (const auto& gun : guns)
	{
		defs.push_back(gun->getGunData());
	}

	auto& data = Data::getInstance();
	data.saveGunData(defs);
}