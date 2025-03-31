#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include "Randomizer.h"

constexpr int MIN_LEAF_SIZE = 10, MAX_LEAF_SIZE = 20;

class Leaf
{
private:
	bool chooseRoom(Leaf*, Leaf*);
	std::vector<Leaf*> allChild;
public:
	sf::IntRect leafData;
	bool isRoom;
	bool isHorRez;
	std::vector<sf::IntRect> halls;
	Leaf* leftChild;
	Leaf* rightChild;
	
	Leaf(sf::Vector2i, sf::Vector2i);
	Leaf() = default;
	~Leaf();

	bool split();
	void creatRooms();
	std::vector<Leaf*> getAllChild();
	Leaf* getRoom();
	void createHall(sf::Vector2i point);
};