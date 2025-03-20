#pragma once
#ifndef UIM

#include "Resources.h"
#include "Player.h"
#include "SFML/Graphics/CircleShape.hpp"

class UIManager
{
public:
	UIManager(sf::RenderWindow* _window);
	void drawPlayerUI(Player* player);
private:
	void init();

	sf::RenderWindow* window;

	std::function<void(Player* player)> playerUI;
};

#endif // UIM