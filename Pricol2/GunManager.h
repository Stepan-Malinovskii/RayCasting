#ifndef GMAG

#include "Resources.h"
#include "Weapon.h"
#include "DataBase.h"

class GunManager
{
public:
	GunManager(Data* data);
	~GunManager();
	Gun* getGun(int index);
private:
	std::vector<Gun*> guns;
	Data* data;
};


#endif // !GMAG

