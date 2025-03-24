#ifndef GMAG
#include "Weapon.h"
#include "DataBase.h"

class GunManager
{
public:
	GunManager(Data* data);
	~GunManager();
	Gun* getGun(int index);
private:
	std::vector<std::unique_ptr<Gun>> guns;
	Data* data;
};

extern std::vector<GunDef> gunsDef;

#endif // !GMAG

