#ifndef GMAG

#include "Resources.h"
#include "Weapon.h"

class GunManager
{
public:
	GunManager();
	~GunManager();
	Gun* getGun(int index);
private:
	void init();
	std::vector<Gun*> guns;
};

#endif // !GMAG

