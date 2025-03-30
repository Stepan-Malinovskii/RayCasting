#pragma once
#ifndef UIM
#define UIM

#include "Player.h"
#include "sfmlExtantion.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "Weapon.h"
#include <map>

constexpr float DIALOG_W = SCREEN_W - 200, DIALOG_H = SCREEN_H - 100, TEXTSIZE = 50, INTERVAL = 40;

class UIManager
{
public:
	UIManager(sf::RenderWindow* _window);
	void drawPlayerUI(Player* player);
	void initDialog(std::map<int, std::wstring, std::greater<int>> variants,
		std::wstring npcName);
	void initTrade(std::map<int, Itemble*> variants, Player* player);
	void initInvent(std::map<Itemble*, int> items, Itemble* chose);
	void deleteNow();
	void drawNow();
	int checkButton(sf::Vector2i mousePos);
private:
	void initPlayer();
	std::wstring splitText(std::wstring text, int maxLen, int textSize);
	std::wstring toMax(std::wstring str, float maxW, float textSize);
	sf::RenderWindow* window;
	Button* choseBut;
	std::vector<Button> buttons;
	int keyButton;
	std::function<void(Player* player)> playerUI;
};
#endif // !UIM
