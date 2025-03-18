#include "Editor.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include "SpriteManager.h"

void Editor::init(sf::RenderWindow& window, sf::RenderWindow& editorWindow, Map* map)
{
	nowMap = map;
	nowValue = 1;
	nowLayer = WALL_LAYER;
	windowView = window.getView();
	editorView = editorWindow.getView();

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
			nowValue = x + 1;
			});

		buttons.push_back(std::make_shared<EdingButton>(b));
	}

	y /= COUNT_ROW_TEXT + 1;
	y = y * (COUNT_ROW_TEXT - 3);

	for (x = 0; x < Resources::spritesTexture.getSize().y / SPRITE_SIZE; x++, y++)
	{
		b = EdingButton(SPRITE_SIZE * (sf::Vector2f(x % (COUNT_ROW_TEXT - 3), y / (COUNT_ROW_TEXT - 3)) + sf::Vector2f(0.025f, 0.025f)),
			{ SPRITE_SIZE, SPRITE_SIZE }, Resources::spritesTexture, { { 0, (int)(SPRITE_SIZE * x)}, {(int)SPRITE_SIZE, (int)SPRITE_SIZE} });
		b.setFunc([=]() {
			nowSpriteDef = spriteDef[x + 1];
			});
		buttons.push_back(std::make_shared<EdingButton>(b));
	}
}

void Editor::takeEditInput(sf::RenderWindow& editorWindow, sf::Event event)
{
	editorMousePos = sf::Mouse::getPosition(editorWindow);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		editorWindowStateLeftClick(editorWindow);
	}

	if (event.type == sf::Event::MouseWheelScrolled)
	{
		float deltaScrol = -5 * event.mouseWheelScroll.delta;
		editorView.move({ 0, deltaScrol });
	}

	editorWindow.setView(editorView);
}

void Editor::takeWindowInput(sf::RenderWindow& window, sf::Event event)
{
	windowMousePos = sf::Mouse::getPosition(window);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		windowStateRightClick(window);
	}
	else
	{
		windowStateNoRightClick(window);
	}
	

	static bool da = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !da)
	{
		da = true;
		windowStateLeftClick(window);
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		da = false;
	}

	if (event.type == sf::Event::MouseWheelScrolled)
	{
		float zoom = 1.0f - 0.1f * event.mouseWheelScroll.delta;
		windowView.zoom(zoom);
	}

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Tab)
		{
			nowLayer++;
			nowLayer = nowLayer % ALL_LAYER;
	}
	}

	window.setView(windowView);
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

			windowView.setCenter(windowView.getCenter() - (sf::Vector2f)deltaMouse);

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
		if (nowLayer != SPRITE_LAYER)
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
				nowMap->deleteMapSprite(mapPos);
			}
			else
			{
				if (nowSpriteDef.texture != -1)
				{
					if (nowMap->isCellEmpty(mapPos))
					{
						nowMap->setMapSprite({ nowSpriteDef.texture + 1, { mapPos.x + 0.5f, mapPos.y + 0.5f }, -90.0f, nowSpriteDef.maxHealpoint });
					}
				}
			}
		}
	}
}

void Editor::editorWindowStateLeftClick(sf::RenderWindow& editorWindow)
{
	if (editorWindow.hasFocus())
	{
		sf::Vector2i worldPos = (sf::Vector2i)editorWindow.mapPixelToCoords(editorMousePos);
		sf::Vector2i mapPos = sf::Vector2i((int)floor(worldPos.x - 0.1f) / TEXTURE_SIZE,
			(int)floor(worldPos.y - 0.1f) / TEXTURE_SIZE);

		for (auto b : buttons)
		{
			if (b->isClicked(worldPos))
			{
				int a;
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

int Editor::drawerLayer() const { return nowLayer; }