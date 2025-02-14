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

	sf::RenderWindow editorWindow(sf::VideoMode(450,500), "Editor");
	editorWindow.setPosition(sf::Vector2i(0, 0));
	editorWindow.setActive(false);
	editorWindow.setVisible(false);

	State state = State::Game;
	
	if (!Resources::textureImage.loadFromFile("Texture/wall_texture.png")) return 1;
	Resources::textures.loadFromImage(Resources::textureImage);
	if (!Resources::spritesTexture.loadFromFile("Texture/enemys1.png")) return 1;
	if (!Resources::skyTextures.loadFromFile("Texture/sky_texture1.png")) return 1;
	Resources::skyTextures.setRepeated(true);

	if (!Resources::gun1BaseTexture.loadFromFile("Texture/weapon.png")) return 1;
	if (!Resources::gun1FireAnimationTexture[0].loadFromFile("Texture/weapon_fire10.png")) return 1;
	if (!Resources::gun1FireAnimationTexture[1].loadFromFile("Texture/weapon_fire11.png")) return 1;
	if (!Resources::gun1FireAnimationTexture[2].loadFromFile("Texture/weapon_fire12.png")) return 1;
	if (!Resources::gun1FireAnimationTexture[3].loadFromFile("Texture/weapon_fire13.png")) return 1;
	if (!Resources::gun1ShutSound.loadFromFile("Sound/gun1ShutSound.mp3")) return 1;
	if (!Resources::gun1ResetSound.loadFromFile("Sound/gun1ResetSound.mp3")) return 1;
	if (!Resources::gun1CantShoutSound.loadFromFile("Sound/gun1CantShutSound.mp3")) return 1;

	if (!Resources::gun2BaseTexture.loadFromFile("Texture/gun2BaseTexture.png")) return 1;
	if (!Resources::gun2FireAnimationTexture[0].loadFromFile("Texture/gun2FireTexture1.png")) return 1;
	if (!Resources::gun2FireAnimationTexture[1].loadFromFile("Texture/gun2FireTexture2.png")) return 1;
	if (!Resources::gun2FireAnimationTexture[2].loadFromFile("Texture/gun2FireTexture3.png")) return 1;
	if (!Resources::gun2FireAnimationTexture[3].loadFromFile("Texture/gun2FireTexture4.png")) return 1;
	if (!Resources::gun2ShutSound.loadFromFile("Sound/gun2ShutSound.mp3")) return 1;
	if (!Resources::gun2ResetSound.loadFromFile("Sound/gun2ResetSound.mp3")) return 1;
	if (!Resources::gun2CantShoutSound.loadFromFile("Sound/gun1CantShutSound.mp3")) return 1;
	if (!Resources::UIFont.loadFromFile("Texture/Pshek_KY2.ttf")) return 1;

	std::unique_ptr<Map> map = std::make_unique<Map>();
	map->LoadGrid("Texture/test.map");
	Editor editor{};
	editor.init(window, editorWindow, map.get());
	Game logic(&window, map.get());



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
						logic.resetMap();
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
				editor.editorEvent(event);
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
			map->Draw(window, editor.drawerLayer());
			editor.drawEditor(editorWindow);
			
			editorWindow.display();
			window.display();
		}

		window.setTitle("Game " + std::to_string(1.0f / deltaTime));
	}

	map->SaveGrid("Texture/test.map");
}