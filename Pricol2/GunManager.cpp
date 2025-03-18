#include "GunManager.h"

GunManager::GunManager() { init(); }

GunManager::~GunManager()
{
	for (auto g : guns)
	{
		delete g;
	}
}

void GunManager::init()
{
	Animation< sf::Texture* > shutAnim01({
		{0.055f, &Resources::gun01FireAnimationTexture[0]},
		{0.110f, &Resources::gun01FireAnimationTexture[1]},
		{0.165f, &Resources::gun01FireAnimationTexture[2]},
		{0.220f, &Resources::gun01FireAnimationTexture[3]},
		{0.275f, &Resources::gun01FireAnimationTexture[3]},
		{0.330f, &Resources::gun01FireAnimationTexture[2]},
		{0.385f, &Resources::gun01FireAnimationTexture[1]},
		{0.440f, &Resources::gun01FireAnimationTexture[0]},
		{0.500f, &Resources::gun01FireAnimationTexture[0]} });
	Animator animr01 = Animator<sf::Texture*>{ nullptr, {shutAnim01} };
	Gun* gun01 = new Gun(10.0f, 100, 0.5f, 1.0f, 0);
	gun01->setAnimator(animr01);
	gun01->setSound(&Resources::gun01ShutSound);
	guns.push_back(gun01);

	Animation< sf::Texture* > shutAnim0({
		{0.00f, &Resources::gun0FireAnimationTexture[0]},
		{0.05f, &Resources::gun0FireAnimationTexture[1]},
		{0.10f, &Resources::gun0FireAnimationTexture[2]},
		{0.15f, &Resources::gun0FireAnimationTexture[2]},
		{0.20f, &Resources::gun0FireAnimationTexture[1]},
		{0.25f, &Resources::gun0FireAnimationTexture[0]},
		{0.30f, &Resources::gun0FireAnimationTexture[0]} });
	Animator animr0 = Animator<sf::Texture*>{ &Resources::gun0BaseTexture, {shutAnim0} };
	Gun* gun0 = new Gun(10.0f, 0, 0.3f, 1.0f, 0);
	gun0->setAnimator(animr0);
	gun0->setShutFunc([&](Sprite* sp, float dist) {sp->spMap.nowHealPoint -= 10.0f * (dist < 1.0f ? 1 : 0);});
	gun0->setSound(&Resources::gun0ShutSound);
	guns.push_back(gun0);

	Animation< sf::Texture* > shutAnim1({
		{0.0f, &Resources::gun1FireAnimationTexture[0]},
		{0.15f, &Resources::gun1FireAnimationTexture[1]},
		{0.3f, &Resources::gun1FireAnimationTexture[2]},
		{0.45f, &Resources::gun1FireAnimationTexture[3]},
		{0.6f, &Resources::gun1FireAnimationTexture[2]},
		{0.75f, &Resources::gun1FireAnimationTexture[1]},
		{0.9f, &Resources::gun1FireAnimationTexture[0]},
		{1.0f, &Resources::gun1FireAnimationTexture[0]} });
	Animation<sf::Texture*> resetAnim1({
	{0.000f, &Resources::gun1ResetAnimationTexture[0]},
	{0.166f, &Resources::gun1ResetAnimationTexture[1]},
	{0.332f, &Resources::gun1ResetAnimationTexture[2]},
	{0.498f, &Resources::gun1ResetAnimationTexture[3]},
	{0.664f, &Resources::gun1ResetAnimationTexture[4]},
	{0.830f, &Resources::gun1ResetAnimationTexture[5]},
	{0.996f, &Resources::gun1ResetAnimationTexture[6]},
	{1.162f, &Resources::gun1ResetAnimationTexture[1]},
	{1.328f, &Resources::gun1ResetAnimationTexture[0]},
	{1.500f, &Resources::gun1ResetAnimationTexture[0]} });
	Animator animr1 = Animator<sf::Texture*>{ &Resources::gun1BaseTexture, {shutAnim1, resetAnim1} };
	Gun* gun1 = new Gun(50.0f, 5, 1.0f, 5.0f, 1.5f);
	gun1->setAnimator(animr1);
	gun1->setResetFunc([&](Gun* gun) {gun->nowCount++;});
	gun1->setShutFunc([&](Sprite* sp, float dist) {sp->spMap.nowHealPoint -= 50.0f * (dist < 5.0f ? 1 : 0);});
	gun1->setSound(&Resources::gun1ShutSound, &Resources::gun1ResetSound, &Resources::gun1CantShoutSound);
	guns.push_back(gun1);

	Animation< sf::Texture* > shutAnim2({
		{0.066f, &Resources::gun2FireAnimationTexture[0]},
		{0.132f, &Resources::gun2FireAnimationTexture[1]},
		{0.198f, &Resources::gun2FireAnimationTexture[2]},
		{0.264f, &Resources::gun2FireAnimationTexture[1]},
		{0.330f, &Resources::gun2FireAnimationTexture[0]},
		{0.400f, &Resources::gun2FireAnimationTexture[0]} });
	Animation<sf::Texture*> resetAnim2({
	{0.000f, &Resources::gun2ResetAnimationTexture[0]},
	{0.214f, &Resources::gun2ResetAnimationTexture[1]},
	{0.428f, &Resources::gun2ResetAnimationTexture[2]},
	{0.642f, &Resources::gun2ResetAnimationTexture[3]},
	{0.856f, &Resources::gun2ResetAnimationTexture[4]},
	{1.070f, &Resources::gun2ResetAnimationTexture[5]},
	{1.284f, &Resources::gun2ResetAnimationTexture[6]},
	{1.500f, &Resources::gun2ResetAnimationTexture[6]} });
	Animator animr2 = Animator<sf::Texture*>{ &Resources::gun2BaseTexture, {shutAnim2, resetAnim2} };
	Gun* gun2 = new Gun(10.0f, 20, 0.4f, 20.0f, 1.5f);
	gun2->setAnimator(animr2);
	gun2->setResetFunc([&](Gun* gun) {gun->nowCount = 20;});
	gun2->setShutFunc([&](Sprite* sp, float dist) {sp->spMap.nowHealPoint -= 10.0f * (dist < 20 ? 1 : 0);});
	gun2->setSound(&Resources::gun2ShutSound, &Resources::gun2ResetSound, &Resources::gun1CantShoutSound);
	guns.push_back(gun2);

	Animation<sf::Texture*> shutAnim3({
		{0.0f, &Resources::gun3FireAnimationTexture[0]},
		{0.1f, &Resources::gun3FireAnimationTexture[1]},
		{0.2f, &Resources::gun3FireAnimationTexture[2]},
		{0.3f, &Resources::gun3FireAnimationTexture[1]},
		{0.4f, &Resources::gun3FireAnimationTexture[0]},
		{0.5f, &Resources::gun3FireAnimationTexture[0]} });
	Animation<sf::Texture*> resetAnim3({
		{0.167f, &Resources::gun3ResetAnimationTexture[0]},
		{0.334f, &Resources::gun3ResetAnimationTexture[1]},
		{0.501f, &Resources::gun3ResetAnimationTexture[2]},
		{0.668f, &Resources::gun3ResetAnimationTexture[3]},
		{0.835f, &Resources::gun3ResetAnimationTexture[4]},
		{1.000f, &Resources::gun3ResetAnimationTexture[5]},
		{1.169f, &Resources::gun3ResetAnimationTexture[6]},
		{1.336f, &Resources::gun3ResetAnimationTexture[7]},
		{1.503f, &Resources::gun3ResetAnimationTexture[8]},
		{1.670f, &Resources::gun3ResetAnimationTexture[9]},
		{1.837f, &Resources::gun3ResetAnimationTexture[10]},
		{2.000f, &Resources::gun3ResetAnimationTexture[11]},
		{2.171f, &Resources::gun3ResetAnimationTexture[12]},
		{2.338f, &Resources::gun3ResetAnimationTexture[13]},
		{2.505f, &Resources::gun3ResetAnimationTexture[14]},
		{2.672f, &Resources::gun3ResetAnimationTexture[15]},
		{2.839f, &Resources::gun3ResetAnimationTexture[16]},
		{3.0f, &Resources::gun3ResetAnimationTexture[0]} });
	Animator animr3 = Animator<sf::Texture*>{ &Resources::gun3BaseTexture, {shutAnim3, resetAnim3} };
	Gun* gun3 = new Gun(100.0f, 2, 0.6f, 5.0f, 3.0f);
	gun3->setAnimator(animr3);
	gun3->setResetFunc([&](Gun* gun) {gun->nowCount = 2;});
	gun3->setShutFunc([&](Sprite* sp, float dist) {sp->spMap.nowHealPoint -= 100.0f * (dist < 5 ? 1 : 0);});
	gun3->setSound(&Resources::gun3ShutSound, &Resources::gun3ResetSound, &Resources::gun1CantShoutSound);
	guns.push_back(gun3);

	Animation<sf::Texture*> shutAnim4({
		{0.05f, &Resources::gun4FireAnimationTexture[0]},
		{0.10f, &Resources::gun4FireAnimationTexture[1]},
		{0.15f, &Resources::gun4FireAnimationTexture[2]},
		{0.20f, &Resources::gun4FireAnimationTexture[2]},
		{0.25f, &Resources::gun4FireAnimationTexture[1]},
		{0.30f, &Resources::gun4FireAnimationTexture[0]},
		});
	Animation<sf::Texture*> resetAnim4({
		{0.25f, &Resources::gun4ResetAnimationTexture[0]},
		{0.50f, &Resources::gun4ResetAnimationTexture[1]},
		{0.75f, &Resources::gun4ResetAnimationTexture[2]},
		{1.00f, &Resources::gun4ResetAnimationTexture[3]},
		{1.25f, &Resources::gun4ResetAnimationTexture[4]},
		{1.50f, &Resources::gun4ResetAnimationTexture[5]},
		{1.75f, &Resources::gun4ResetAnimationTexture[6]},
		{2.00f, &Resources::gun4ResetAnimationTexture[7]},
		{2.25f, &Resources::gun4ResetAnimationTexture[8]},
		{2.50f, &Resources::gun4ResetAnimationTexture[9]},
		{2.75f, &Resources::gun4ResetAnimationTexture[10]},
		{2.85f, &Resources::gun4ResetAnimationTexture[8]},
		{3.0f, &Resources::gun4ResetAnimationTexture[8]} });
	Animator animr4 = Animator<sf::Texture*>{ &Resources::gun4BaseTexture, {shutAnim4, resetAnim4} };
	Gun* gun4 = new Gun(15.0f, 30, 0.3f, 15.0f, 3.0f);
	gun4->setAnimator(animr4);
	gun4->setResetFunc([&](Gun* gun) {gun->nowCount = 30;});
	gun4->setShutFunc([&](Sprite* sp, float dist) {sp->spMap.nowHealPoint -= 15.0f * (dist < 15 ? 1 : 0);});
	gun4->setSound(&Resources::gun4ShutSound, &Resources::gun4ResetSound, &Resources::gun1CantShoutSound);
	guns.push_back(gun4);

	Animation<sf::Texture*> shutAnim5({
		{0.0f, &Resources::gun5FireAnimationTexture[0]},
		{0.2f, &Resources::gun5FireAnimationTexture[1]},
		{0.4f, &Resources::gun5FireAnimationTexture[2]},
		{0.6f, &Resources::gun5FireAnimationTexture[1]},
		{0.8f, &Resources::gun5FireAnimationTexture[0]},
		{1.0f, &Resources::gun5FireAnimationTexture[0]} });
	Animation<sf::Texture*> resetAnim5({
		{0.21f, &Resources::gun5ResetAnimationTexture[0]},
		{0.42f, &Resources::gun5ResetAnimationTexture[1]},
		{0.63f, &Resources::gun5ResetAnimationTexture[2]},
		{0.84f, &Resources::gun5ResetAnimationTexture[3]},
		{1.05f, &Resources::gun5ResetAnimationTexture[4]},
		{1.26f, &Resources::gun5ResetAnimationTexture[5]},
		{1.47f, &Resources::gun5ResetAnimationTexture[6]},
		{1.68f, &Resources::gun5ResetAnimationTexture[7]},
		{1.89f, &Resources::gun5ResetAnimationTexture[8]},
		{2.10f, &Resources::gun5ResetAnimationTexture[9]},
		{2.31f, &Resources::gun5ResetAnimationTexture[10]},
		{2.52f, &Resources::gun5ResetAnimationTexture[11]},
		{2.73f, &Resources::gun5ResetAnimationTexture[12]},
		{3.00f, &Resources::gun5ResetAnimationTexture[1]} });
	Animator animr5 = Animator<sf::Texture*>{ &Resources::gun5BaseTexture, {shutAnim5, resetAnim5} };
	Gun* gun5 = new Gun(200.0f, 1, 1.0f, 10.0f, 3.0f);
	gun5->setAnimator(animr5);
	gun5->setResetFunc([&](Gun* gun) {gun->nowCount = 1;});
	gun5->setShutFunc([&](Sprite* sp, float dist) {sp->spMap.nowHealPoint -= 200.0f * (dist < 10 ? 1 : 0);});
	gun5->setSound(&Resources::gun5ShutSound, &Resources::gun5ResetSound, &Resources::gun1CantShoutSound);
	guns.push_back(gun5);

	Animation<sf::Texture*> shutAnim6({
		{0.02f, &Resources::gun6FireAnimationTexture[0]},
		{0.04f, &Resources::gun6FireAnimationTexture[1]},
		{0.06f, &Resources::gun6FireAnimationTexture[2]},
		{0.08f, &Resources::gun6FireAnimationTexture[3]},
		{0.1f, &Resources::gun6FireAnimationTexture[3]} });
	Animation<sf::Texture*> resetAnim6({
		{0.21f, &Resources::gun6ResetAnimationTexture[0]},
		{0.42f, &Resources::gun6ResetAnimationTexture[1]},
		{0.63f, &Resources::gun6ResetAnimationTexture[2]},
		{0.84f, &Resources::gun6ResetAnimationTexture[3]},
		{1.05f, &Resources::gun6ResetAnimationTexture[4]},
		{1.26f, &Resources::gun6ResetAnimationTexture[5]},
		{1.47f, &Resources::gun6ResetAnimationTexture[6]},
		{2.74f, &Resources::gun6ResetAnimationTexture[5]},
		{2.95f, &Resources::gun6ResetAnimationTexture[4]},
		{3.16f, &Resources::gun6ResetAnimationTexture[3]},
		{3.37f, &Resources::gun6ResetAnimationTexture[2]},
		{3.58f, &Resources::gun6ResetAnimationTexture[1]},
		{3.79f, &Resources::gun6ResetAnimationTexture[0]},
		{4.00f, &Resources::gun6ResetAnimationTexture[0]} });
	Animator animr6 = Animator<sf::Texture*>{ &Resources::gun6BaseTexture, {shutAnim6, resetAnim6 } };
	Gun* gun6 = new Gun(15.0f, 100, 0.1f, 10.0f, 4.0f);
	gun6->setAnimator(animr6);
	gun6->setResetFunc([&](Gun* gun) {gun->nowCount = 100;});
	gun6->setShutFunc([&](Sprite* sp, float dist) {sp->spMap.nowHealPoint -= 10.0f * (dist < 10 ? 1 : 0);});
	gun6->setSound(&Resources::gun6ShutSound, &Resources::gun6ResetSound, &Resources::gun1CantShoutSound);
	guns.push_back(gun6);
}

Gun* GunManager::getGun(int index) { return guns[index]; }