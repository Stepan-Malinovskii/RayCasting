#pragma once
#ifndef EDITOR
#define EDITOR

#include "Map.h"
#include "Resources.h"

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

constexpr int COUNT_ROW_TEXT = 5;

class Editor
{
public:
	void init(sf::RenderWindow& window, Map* map);
	void takeInput(sf::RenderWindow& window, sf::RenderWindow& editorWindow);
	void windowEvent(const sf::Event& event);
	void drawEditor(sf::RenderWindow& editorWindow);

	int drawerLayer() const;
private:
	bool isFirstMouse;
	int nowValue;
	int nowLayer;
	sf::RectangleShape cellShape;
	sf::Vector2i lastMousePos;
	sf::Vector2i windowMousePos, editorMousePos;
	std::vector<int> textures;
	sf::View view;
	Map* nowMap;

	void windowStateRightClick(sf::RenderWindow& window);
	void windowStateNoRightClick(sf::RenderWindow& window);
	void windowStateLeftClick(sf::RenderWindow& window);
	void editorWindowStateLeftClick(sf::RenderWindow& editorWindow);
};

#endif // !EDITOR