#pragma once
#ifndef UIM
#include "Player.h"
#include "sfmlExtantion.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <sstream>
#include <iomanip>

constexpr float DIALOG_W = SCREEN_W - 200, DIALOG_H = SCREEN_H - 100, TEXTSIZE = 50, INTERVAL = 40;

class UIManager
{
public:
	UIManager(sf::RenderWindow* _window);
	void drawPlayerUI(Player* player);
	void initDialog(std::vector<std::pair<std::wstring, int>> variants, 
		std::wstring npcName);
	void initTrade(std::map<int, Itemble*> variants);
	void deleteNow();
	void drawNow();
	int checkButton(sf::Vector2i mousePos);
private:
	void initPlayer();
	std::wstring splitText(std::wstring text, int maxLen, int textSize);
	sf::RenderWindow* window;
	std::vector<Button> buttons;
	int keyButton;
	std::function<void(Player* player)> playerUI;
};
#endif // !UIM
