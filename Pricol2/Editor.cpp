#include "Editor.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

void Editor::init(sf::RenderWindow& window, Map* map)
{
	nowMap = map;
	nowValue = 1;
	nowLayer = WALL_LAYER;
	view = window.getView();
	cellShape.setSize(sf::Vector2f(TEXTURE_SIZE, TEXTURE_SIZE));
	cellShape.setFillColor(sf::Color::Cyan);
	
	textures = std::vector(Resources::textures.getSize().x / TEXTURE_SIZE, 0);
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i] = i + 1;
	}
}

void Editor::takeInput(sf::RenderWindow& window, sf::RenderWindow& editorWindow)
{
	windowMousePos = sf::Mouse::getPosition(window);
	editorMousePos = sf::Mouse::getPosition(editorWindow);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		windowStateRightClick(window);
	}
	else
	{
		windowStateNoRightClick(window);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		windowStateLeftClick(window);
		editorWindowStateLeftClick(editorWindow);
	}

	window.setView(view);
}

void Editor::windowStateRightClick(sf::RenderWindow& window)
{
	if (window.hasFocus())
	{
		if (isFirstMouse)
		{
			lastMousePos = windowMousePos;
			isFirstMouse = false;
		}
		else
		{
			sf::Vector2i deltaMouse = windowMousePos - lastMousePos;

			view.setCenter(view.getCenter() - (sf::Vector2f)deltaMouse);

			sf::Mouse::setPosition(lastMousePos, window);
		}
		window.setMouseCursorVisible(false);
	}
}

void Editor::windowStateNoRightClick(sf::RenderWindow& window)
{
	if (window.hasFocus())
	{
		isFirstMouse = true;
		window.setMouseCursorVisible(true);
	}
}

void Editor::windowStateLeftClick(sf::RenderWindow& window)
{
	if (window.hasFocus())
	{
		sf::Vector2f worldPos = window.mapPixelToCoords(windowMousePos);
		sf::Vector2i mapPos = sf::Vector2i((int)floor(worldPos.x - 0.1f) / TEXTURE_SIZE,
			(int)floor(worldPos.y - 0.1f) / TEXTURE_SIZE);
		cellShape.setPosition((sf::Vector2f)mapPos * TEXTURE_SIZE);
		window.draw(cellShape);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
		{
			nowMap->SetNewOnGrid(mapPos.x, mapPos.y, nowLayer, 0);
		}
		else
		{
			nowMap->SetNewOnGrid(mapPos.x, mapPos.y, nowLayer, nowValue);
		}
	}
}

void Editor::editorWindowStateLeftClick(sf::RenderWindow & editorWindow)
{
	if (editorWindow.hasFocus())
	{
		sf::Vector2f worldPos = editorWindow.mapPixelToCoords(editorMousePos);
		sf::Vector2i mapPos = sf::Vector2i((int)floor(worldPos.x - 0.1f) / TEXTURE_SIZE,
			(int)floor(worldPos.y - 0.1f) / TEXTURE_SIZE);
		nowValue = mapPos.x + mapPos.y * COUNT_ROW_TEXT+ 1;
		cellShape.setPosition((sf::Vector2f)mapPos * TEXTURE_SIZE);
		editorWindow.draw(cellShape);
	}
}

void Editor::drawEditor(sf::RenderWindow& editorWindow)
{
	sf::RectangleShape textSh(sf::Vector2f(TEXTURE_SIZE * 0.95f, TEXTURE_SIZE * 0.95f));
	textSh.setTexture(&Resources::textures);

	int y = 0;
	for (int x = 0; x < textures.size(); x++, y++)
	{
		textSh.setTextureRect(sf::IntRect(sf::Vector2i((textures[x] - 1) * TEXTURE_SIZE, 0.0f),
			sf::Vector2i(TEXTURE_SIZE, TEXTURE_SIZE)));
		textSh.setPosition(TEXTURE_SIZE * (sf::Vector2f(x % COUNT_ROW_TEXT, y / COUNT_ROW_TEXT) + sf::Vector2f(0.025f, 0.025f)));
		editorWindow.draw(textSh);
	}
}

void Editor::windowEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		float zoom = 1.0f - 0.1f * event.mouseWheelScroll.delta;
		view.zoom(zoom);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
	{
		nowLayer++;
		nowLayer = nowLayer % LAYER_COUNT;
	}
}

int Editor::drawerLayer() const { return nowLayer; }