#include "Map.h"
#include "Editor.h"
#include "Renderer.h"
#include "Resources.h"
#include "Game.h"

enum class State{Editor, Game};

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H),"Game");
	window.setMouseCursorVisible(false);

	sf::RenderWindow editorWindow(sf::VideoMode(500,500), "Editor");
	editorWindow.setPosition(sf::Vector2i(0, 0));
	editorWindow.setActive(false);
	editorWindow.setVisible(false);

	State state = State::Game;
	
	if (!Resources::playerGunTexture.loadFromFile("Texture/player_gun.png")) return 1;
	if (!Resources::textureImage.loadFromFile("Texture/wall_texture.png")) return 1;
	Resources::textures.loadFromImage(Resources::textureImage);
	if (!Resources::spritesTexture.loadFromFile("Texture/enemys.png")) return 1;
	if (!Resources::skyTextures.loadFromFile("Texture/sky_texture1.png")) return 1;
	Resources::skyTextures.setRepeated(true);

	Map map{};
	map.LoadGrid("Texture/test.map");
	Editor editor{};
	editor.init(window, &map);
	Game logic(&window, &map);

	sf::Clock gameClock;

	float deltaTime;
	while (window.isOpen())
	{
		deltaTime = gameClock.getElapsedTime().asSeconds();
		gameClock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					if (state == State::Editor)
					{
						state = State::Game;
						window.setMouseCursorVisible(false);
						window.setView(window.getDefaultView());

						editorWindow.setActive(false);
						editorWindow.setVisible(false);
					}
					else
					{
						state = State::Editor;
						window.setMouseCursorVisible(true);
						editorWindow.setVisible(true);
						editorWindow.setActive(true);
					}

				}
			}

			if (state == State::Editor)
			{
				editor.windowEvent(event);
			}
		}

		if (state == State::Editor)
		{
			while (editorWindow.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					editorWindow.close();
				}
			}
		}

		if (state == State::Game)
		{
			logic.update(deltaTime);
			logic.render();
		}
		else
		{	
			editorWindow.clear();
			window.clear();

			editor.takeInput(window, editorWindow);
			map.Draw(window, editor.drawerLayer());
			editor.drawEditor(editorWindow);
			
			editorWindow.display();
			window.display();
		}

		window.setTitle("Game " + std::to_string(1.0f / deltaTime));
	}

	map.SaveGrid("Texture/test.map");
}