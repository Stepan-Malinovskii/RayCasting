#include "Weapon.h"

Weapon::Weapon(float _timeBetewen, sf::Texture& baseText, std::vector<Animation<sf::Texture*>> animation)
	: timeBetwen{ _timeBetewen }, weaponTexture{ baseText }{
	nowTime = _timeBetewen;
	std::array<sf::Texture, 4> weapon1Texture;
	sf::Texture baseWeaponText;
	if (!baseWeaponText.loadFromFile("Texture/weapon.png")) exit(2);
	if (!weapon1Texture[0].loadFromFile("Texture/weapon_fire10.png")) exit(2);
	if (!weapon1Texture[1].loadFromFile("Texture/weapon_fire11.png")) exit(2);
	if (!weapon1Texture[2].loadFromFile("Texture/weapon_fire12.png")) exit(2);
	if (!weapon1Texture[3].loadFromFile("Texture/weapon_fire13.png")) exit(2);

	weaponAnimator = Animator{&baseWeaponText, { Animation<sf::Texture*>({
		{0.0f, &weapon1Texture[0]},
		{0.15f, &weapon1Texture[1]},
		{0.3f, &weapon1Texture[2]},
		{0.45f, &weapon1Texture[3]},
		{0.6f, &weapon1Texture[2]},
		{0.75f, &weapon1Texture[1]},
		{0.9f, &weapon1Texture[0]},
		{1.0f, &weapon1Texture[0]},

}) } };
}

