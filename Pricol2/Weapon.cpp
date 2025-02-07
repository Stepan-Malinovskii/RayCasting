#include "Weapon.h"

Weapon::Weapon(float _timeBetewen, Animator<sf::Texture*> anim)
	: timeBetwen{ _timeBetewen }, weaponAnimator{ anim } {
	nowTime = _timeBetewen;
}

