#include "Map.h"
#include "Editor.h"
#include "Renderer.h"
#include "Resources.h"
#include "Game.h"
#include "SpriteManager.h"

enum class State{Editor, Game};

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H),"Game");
	window.setFramerateLimit(900);
	window.setMouseCursorVisible(false);

	sf::RenderWindow editorWindow(sf::VideoMode(450,500), "Editor");
	editorWindow.setPosition(sf::Vector2i(0, 0));
	editorWindow.setActive(false);
	editorWindow.setVisible(false);

	State state = State::Game;
	
	Resources::initResources();
	std::unique_ptr<Map> map = std::make_unique<Map>();
	map->LoadGrid("Texture/test.map");
	Editor editor{};
	editor.init(window, editorWindow, map.get());
	Game game(&window, map.get());

	sf::Clock gameClock;

	float deltaTime = 0;
	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (state == State::Editor)
			{
				editor.takeWindowInput(window, event);
			}
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
						game.resetMap(map.get());
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

			if (state == State::Game)
			{
				game.getInput(event, deltaTime);
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
				editor.takeEditInput(editorWindow, event);
			}
		}

		if (state == State::Game)
		{
			game.makeCycle(deltaTime);
			window.display();
		}
		else
		{	
			editorWindow.clear();
			window.clear();
			map->Draw(window, editor.drawerLayer());
			editor.drawEditor(editorWindow);
			
			editorWindow.display();
			window.display();
		}

		window.setTitle("Game " + std::to_string(1.0f / deltaTime));
		deltaTime = gameClock.getElapsedTime().asSeconds();
		gameClock.restart();
	}
	game.save();
	map->SaveGrid("Texture/test.map");
}