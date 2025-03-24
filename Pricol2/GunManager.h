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

static std::vector<GunDef> gunsDef{
	{0, 10, 100, 100, 0.5f, 1.0f, 0},
	{1, 10, 100, 100, 0.3f, 1.0f, 0},
	{2, 50, 5, 5, 1.0f, 5.0f, 1.5f},
	{3, 10, 20, 20, 0.4f, 20.0f, 1.5f},
	{4, 100, 2, 2, 0.6f, 5.0f, 3.0f},
	{5, 15, 30, 30, 0.4f, 15.0f, 3.0f},
	{6, 200, 1, 1, 1.0f, 10.0f, 3.0f},
	{7, 15, 100, 100, 0.1f, 10.0f, 4.0f},
};

#endif // !GMAG

