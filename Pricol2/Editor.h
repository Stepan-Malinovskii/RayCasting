#pragma once
#ifndef EDITOR
#define EDITOR

#include "Map.h"
#include "Resources.h"
#include "sfmlExtantion.h"

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <functional>

constexpr int COUNT_ROW_TEXT = 7;

class Editor
{
public:
	void init(sf::RenderWindow& window, sf::RenderWindow& editorWindow, Map* map);
	void takeWindowInput(sf::RenderWindow& window, sf::Event event);
	void takeEditInput(sf::RenderWindow& editorWindow, sf::Event event);
	void drawEditor(sf::RenderWindow& editorWindow);

	int drawerLayer() const;
private:
	bool isFirstMouse;
	int nowValue;
	int nowLayer;
	SpriteDef nowSpriteDef;
	sf::Vector2i lastMousePos;
	sf::Vector2i windowMousePos, editorMousePos;
	sf::View windowView, editorView;
	Map* nowMap;
	std::vector<std::shared_ptr<Button>> buttons;

	void initButton();
	void windowStateRightClick(sf::RenderWindow& window);
	void windowStateNoRightClick(sf::RenderWindow& window);
	void windowStateLeftClick(sf::RenderWindow& window);
	void editorWindowStateLeftClick(sf::RenderWindow& editorWindow);
};

#endif // !EDITOR