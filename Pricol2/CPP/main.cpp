#include "Editor.h"
#include "Resources.h"
#include "Game.h"
#include "MapManager.h"
#include <sfeMovie/Movie.hpp>

enum class State{Editor, Game};

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H), "Game");
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

	sf::RenderWindow editorWindow(sf::VideoMode(450,500), "Editor");
	editorWindow.setPosition(sf::Vector2i(0, 0));
	editorWindow.setActive(false);
	editorWindow.setVisible(false);

	State state = State::Game;
	Resources::initResources();
	auto& event = EventSystem::getInstance();
	event.subscribe<int>("RESET_GAME", [&](const int NON) {
		sfe::Movie movie;
		if (!movie.openFromFile("Sound/startIntroVideo.mp4")) return;
		movie.fit(0.0f, 0.0f, (float)window.getSize().x, (float)window.getSize().y);
		movie.play();
		SoundManager::stopAllSound();
		SoundManager::playerMusic(StartIntro);

		while (movie.getStatus() == sfe::Status::Playing) // ������� � �������� ������
		{
			window.clear();	
			movie.update();
			window.draw(movie);
			window.display();
		}

		window.clear();
		});

	event.subscribe<int>("WIN_GAME", [&](const int NON) {
		sfe::Movie movie;
		if (!movie.openFromFile("Sound/endIntroVideo.mp4")) return;
		movie.fit(0.0f, 0.0f, (float)window.getSize().x, (float)window.getSize().y);
		movie.play();
		SoundManager::stopAllSound();
		SoundManager::playerMusic(EndIntro);

		while (movie.getStatus() == sfe::Status::Playing) // ������� � �������� ������
		{
			window.clear();
			movie.update();
			window.draw(movie);
			window.display();
		}

		window.clear();
		});

	std::unique_ptr<MapManager> mapManager = std::make_unique<MapManager>(&window);
	mapManager->load();

	std::unique_ptr<Editor> editor = std::make_unique<Editor>();
	editor->init(&window, &editorWindow, mapManager.get());

	std::unique_ptr<Game> game = std::make_unique<Game>(&window, mapManager.get());

	sf::Clock gameClock;
	float deltaTime = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (state == State::Editor)
			{
				editor->takeWindowInput(event);
			}
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::RControl)
				{
					if (state == State::Editor)
					{
						state = State::Game;
						window.setMouseCursorVisible(false);
						window.setView(window.getDefaultView());
						game->editor();
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
				game->getInput(event, deltaTime);
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
				editor->takeEditInput(event);
			}
		}

		window.clear();

		if (state == State::Game)
		{
			game->makeCycle(deltaTime);
		}
		else
		{	
			editorWindow.clear();

			mapManager->drawMap(editor->drawerLayer());
			editor->drawEditor();
			
			editorWindow.display();
		}

		window.display();
		window.setTitle("SOLDIER " + std::to_string(1.0f / deltaTime));
		deltaTime = gameClock.getElapsedTime().asSeconds();
		gameClock.restart();
	}

	game->save();

	return 0;
}