#include "GunManager.h"

GunManager::GunManager(Data* _data)
{
	data = _data;

	for (int i = 0; i < improveDefs.size(); i++)
	{
		improvements.push_back(std::make_unique<Improve>(improveDefs[i]));
	}

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
		guns.push_back(std::make_unique<Gun>(def, i > 1));
		guns.back()->setAnimator(animr);
		guns.back()->setSound(&Resources::gunsShutSound[i],
			&Resources::gunsResetSound[i],
			&Resources::gunCantShoutSound);
	}

	guns[2]->trySetImprove(improvements[4].get());
}

GunManager::~GunManager() {}

Gun* GunManager::getGun(int index) { return guns[index].get(); }