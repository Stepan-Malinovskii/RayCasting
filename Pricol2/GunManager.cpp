#include "GunManager.h"

std::vector<GunDef> gunsDef{
	{0, 10, 100, 100, 0.5f, 1.0f, 0},
	{1, 10, 100, 100, 0.3f, 1.0f, 0},
	{2, 50, 5, 5, 1.0f, 5.0f, 1.5f},
	{3, 10, 20, 20, 0.4f, 20.0f, 1.5f},
	{4, 100, 2, 2, 0.6f, 5.0f, 3.0f},
	{5, 15, 30, 30, 0.4f, 15.0f, 3.0f},
	{6, 200, 1, 1, 1.0f, 10.0f, 3.0f},
	{7, 15, 100, 100, 0.1f, 10.0f, 4.0f},
};

GunManager::GunManager(Data* _data) 
{
	data = _data;
	Gun gun;
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
		guns.push_back(std::make_unique<Gun>(def));
		guns.back()->setAnimator(animr);
		if (resetAnim.getDuration() != 0.0f ) 
			guns.back()->setResetFunc([=]() {guns[i]->nowCount = gunsDef[i].maxCount;});
		if (shutAnim.getDuration() != 0.0f ) 
			guns.back()->setShutFunc([=](Sprite* sp, float dist){
			sp->spMap.nowHealPoint -= guns[i]->damage *
				(dist < guns[i]->maxDist ? 1 : 0);});
		guns.back()->setSound(&Resources::gunsShutSound[i],
							  &Resources::gunsResetSound[i], 
							  &Resources::gunCantShoutSound);
	}
}

GunManager::~GunManager() {}

Gun* GunManager::getGun(int index) { return guns[index].get(); }