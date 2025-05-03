#include "Leaf.h"
Leaf::Leaf(sf::Vector2i pos, sf::Vector2i size)
{
	leftChild = nullptr;
	rightChild = nullptr;
	leafData = sf::IntRect(pos.x, pos.y, size.x, size.y);
	isRoom = false;
	isHorRez = false;
}

Leaf::~Leaf()
{
	delete leftChild;
	delete rightChild;
}

std::vector<Leaf*>  Leaf::getAllChild()
{	
	if (leftChild != nullptr)
	{
		if (leftChild->isRoom) 
		{ 
			allChild.push_back(leftChild);
		}
		else
		{
			auto data = leftChild->getAllChild();
			for (auto d : data)
			{
				allChild.push_back(d);
			}
		}
	}

	if (rightChild != nullptr)
	{
		if (rightChild->isRoom) 
		{ 
			allChild.push_back(leftChild);
		}
		else
		{
			auto data = rightChild->getAllChild();
			for (auto d : data)
			{
				allChild.push_back(d);
			}
		}
	}
	return allChild;
}

bool Leaf::split()
{
	if (leftChild != nullptr || rightChild != nullptr) { return false; }

	auto a = Random::bitRandom();
	bool splitHor = a > 0.5f;
	if (leafData.width > leafData.height && leafData.width / (float)leafData.height >= 1.25f) {
		splitHor = false;
	}
	else if (leafData.height > leafData.width && leafData.height / (float)leafData.width >= 1.25f) {
		splitHor = true;
	}

	int max = (splitHor ? leafData.height : leafData.width) - MIN_LEAF_SIZE;
	if (max <= MIN_LEAF_SIZE) { return false; }
	isHorRez = splitHor;
	int valRez = Random::intRandom(MIN_LEAF_SIZE, max);

	if (splitHor)
	{
		leftChild =  new Leaf({leafData.left, leafData.top}, {leafData.width, valRez});
		rightChild = new Leaf({ leafData.left, leafData.top + valRez}, { leafData.width, leafData.height-valRez });
	}
	else
	{
		leftChild = new Leaf({ leafData.left, leafData.top }, { valRez, leafData.height });
		rightChild = new Leaf({ leafData.left + valRez, leafData.top }, { leafData.width - valRez, leafData.height });
	}
	return true;
}

bool Leaf::chooseRoom(Leaf* a, Leaf* b)
{
	auto childs = a->getAllChild();
	Leaf* l;
	Leaf* r;
	sf::Vector2i point{ 0,0 };

	if (childs.size() != 0)
	{
		float minDist = FLT_MAX;
		int index = 0;
		for (int i = 0; i < childs.size(); i++)
		{
			auto dist = std::min(minDist, (float)sqrt(pow(b->leafData.left - childs[i]->leafData.left, 2) +
				pow(b->leafData.top - childs[i]->leafData.top, 2)));
			if (minDist > dist)
			{
				minDist = dist;
				index = i;
			}
		}
		l = childs[index];
		r = b;
	}
	else
	{
		l = a;
		r = b;
	}

	if (isHorRez)
	{
		point.y = std::max(l->leafData.top, r->leafData.top) - 1;

		if (l->leafData.left < r->leafData.left)
		{
			if (l->leafData.left + l->leafData.width < r->leafData.left + r->leafData.width)
			{
				if (l->leafData.left + l->leafData.width - r->leafData.left > 2)
				{
					point.x = Random::intRandom(r->leafData.left + 2, l->leafData.left + l->leafData.width - 3);
				}
			}
			else
			{
				if (r->leafData.left + r->leafData.width - r->leafData.left > 2)
				{
					point.x = Random::intRandom(r->leafData.left + 2, r->leafData.left + r->leafData.width - 3);
				}
			}
		}
		else
		{
			if (l->leafData.left + l->leafData.width < r->leafData.left + r->leafData.width)
			{
				if (l->leafData.left + l->leafData.width - l->leafData.left > 2)
				{
					point.x = Random::intRandom(l->leafData.left + 2, l->leafData.left + l->leafData.width - 3);
				}
			}
			else
			{
				if (r->leafData.left + r->leafData.width - l->leafData.left > 2)
				{
					point.x = Random::intRandom(l->leafData.left + 2, r->leafData.left + r->leafData.width - 3);
				}
			}
		}
	}
	else
	{
		point.x = std::max(l->leafData.left, r->leafData.left) - 1;

		if (l->leafData.top < r->leafData.top)
		{
			if (l->leafData.top + l->leafData.height < r->leafData.top + r->leafData.height)
			{
				if (l->leafData.top + l->leafData.height - r->leafData.top > 2)
				{
					point.y = Random::intRandom(r->leafData.top + 2, l->leafData.top + l->leafData.height - 3);
				}
			}
			else
			{
				if (r->leafData.top + r->leafData.height - r->leafData.top > 2)
				{
					point.y = Random::intRandom(r->leafData.top + 2, r->leafData.top + r->leafData.height - 3);
				}
			}
		}
		else
		{
			if (l->leafData.top + l->leafData.height < r->leafData.top + r->leafData.height)
			{
				if (l->leafData.top + l->leafData.height - l->leafData.top > 2)
				{
					point.y = Random::intRandom(l->leafData.top + 2, l->leafData.top + l->leafData.height - 3);
				}
			}
			else
			{
				if (r->leafData.top + r->leafData.height - l->leafData.top > 2)
				{
					point.y = Random::intRandom(l->leafData.top + 2, r->leafData.top + r->leafData.height - 3);
				}
			}
		}
	}

	if (point == sf::Vector2i{0, 0})
	{
		return false;
	}

	createHall(point);

	return true;
}

void Leaf::creatRooms()
{
	if (leftChild != nullptr || rightChild != nullptr)
	{
		if (leftChild != nullptr)
		{
			leftChild->creatRooms();
		}
		if (rightChild != nullptr)
		{
			rightChild->creatRooms();
		}
		if (rightChild != nullptr && leftChild != nullptr)
		{
			if (rightChild->isRoom && leftChild->isRoom)
			{
				chooseRoom(leftChild, rightChild);
			}
			else if (rightChild->isRoom)
			{
				chooseRoom(leftChild, rightChild);
			}
			else if (leftChild->isRoom)
			{
				chooseRoom(rightChild, leftChild);
			}
			else
			{
				if (!chooseRoom(rightChild, leftChild->leftChild))
				{
					if (!chooseRoom(rightChild, leftChild->rightChild))
					{
						if (!chooseRoom(leftChild, rightChild->leftChild))
						{
							if (!chooseRoom(leftChild, rightChild->rightChild))
							{
								throw "Generation error";
							}
						}
					}
				}
			}
		}
	}
	else
	{
		isRoom = true;
	}
}

Leaf* Leaf::getRoom()
{
	if (isRoom)
	{
		return this;
	}
	else
	{
		Leaf* lRoom = nullptr;
		Leaf* rRoom = nullptr;
		if (leftChild != nullptr)
		{
			lRoom = leftChild->getRoom();
		}
		if (rightChild != nullptr)
		{
			rRoom = rightChild->getRoom();
		}

		if (lRoom == nullptr && rRoom == nullptr)
		{
			return nullptr;
		}
		else if (rRoom == nullptr)
		{
			return lRoom;
		}
		else if (lRoom == nullptr)
		{
			return rRoom;
		}
		else
		{
			if (Random::bitRandom() > 0.5f)
			{
				return lRoom;
			}
			else
			{
				return rRoom;
			}
		}
	}
}

void Leaf::createHall(sf::Vector2i point)
{
	sf::Vector2i size;

	if (isHorRez)
	{
		size = { 1, 2 };
	}
	else
	{ 
		size = { 2, 1 };
	}

	halls.push_back({point,{size}});
}