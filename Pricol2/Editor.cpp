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

	initButton();
}

void Editor::initButton()
{
	EdingButton b;
	int x = 0, y = 0;
	for (; x < Resources::textures.getSize().x / TEXTURE_SIZE; x++, y++)
	{ 
		b = EdingButton(TEXTURE_SIZE * (sf::Vector2f(x % COUNT_ROW_TEXT, y / COUNT_ROW_TEXT) + sf::Vector2f(0.025f, 0.025f)),
			{ TEXTURE_SIZE * 0.95, TEXTURE_SIZE * 0.95, }, Resources::textures, { {(int)(x * TEXTURE_SIZE), 0}, {(int)TEXTURE_SIZE, (int)TEXTURE_SIZE} });
		b.setFunc([=]() { 
			spriteMode = false;
			nowValue = x + 1;
			});
		
		buttons.push_back(std::make_shared<EdingButton>(b));
	}
	int lastX = x;
	
	for (x = 0, y += y % COUNT_ROW_TEXT; x < Resources::spritesTexture.getSize().x / SPRITE_SIZE; x++, y++)
	{
		b = EdingButton(SPRITE_SIZE * (sf::Vector2f(x % COUNT_ROW_TEXT, y / COUNT_ROW_TEXT) + sf::Vector2f(0.025f, 0.025f)),
			{ SPRITE_SIZE, SPRITE_SIZE}, Resources::spritesTexture, { { (int)(x * SPRITE_SIZE), 0 }, {(int)SPRITE_SIZE, (int)SPRITE_SIZE} });
		b.setFunc([=]() { 
			spriteMode = true;
			nowSpriteDef = spriteDef[x + 1];
			});
		buttons.push_back(std::make_shared<EdingButton>(b));
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
	{
		if (window.hasFocus())
		{
			nowLayer++;
			nowLayer = nowLayer % LAYER_COUNT;
		}
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
		if (!spriteMode)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
			{
				nowMap->SetNewOnGrid(mapPos.x, mapPos.y, nowLayer, 0);
			}
			else
			{
				nowMap->SetNewOnGrid(mapPos.x, mapPos.y, nowLayer, nowValue);
			}
		}
		else
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
			{
				nowMap->deleteSprite(mapPos);
			}
			else
			{
				Sprite sp(nowSpriteDef, { nowSpriteDef.indexTexture, (sf::Vector2f)mapPos, 0.0f });
				nowMap->setSprites(sp);
			}
		}
	}
}

void Editor::editorWindowStateLeftClick(sf::RenderWindow& editorWindow)
{
	if (editorWindow.hasFocus())
	{
		sf::Vector2f worldPos = editorWindow.mapPixelToCoords(editorMousePos);
		sf::Vector2i mapPos = sf::Vector2i((int)floor(worldPos.x - 0.1f) / TEXTURE_SIZE,
			(int)floor(worldPos.y - 0.1f) / TEXTURE_SIZE);
		cellShape.setPosition((sf::Vector2f)mapPos * TEXTURE_SIZE);
		editorWindow.draw(cellShape);

		for (auto b : buttons)
		{
			if (b->isClicked(editorMousePos))
			{
				b->update();
			}
		}
	}
}

void Editor::drawEditor(sf::RenderWindow& editorWindow)
{		
	for (auto b : buttons)
	{
		b->drawButton(editorWindow);
	}
}

void Editor::windowEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		float zoom = 1.0f - 0.1f * event.mouseWheelScroll.delta;
		view.zoom(zoom);
	}
}

int Editor::drawerLayer() const { return nowLayer; }
