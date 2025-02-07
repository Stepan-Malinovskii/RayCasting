#include "Game.h"

Game::Game(sf::RenderWindow* _window, Map* _nowMap) :
	renderer(), window{ _window }, nowMap{ _nowMap }
{
	for (auto sp : nowMap->getMapSprites())
	{
		auto def = spriteDef[sp.idx];
		auto sprite = std::make_shared<Sprite>(def, sp);

		if (sp.idx == 0) {
			player = std::make_unique<Player>(sprite.get());
		}
		else
		{
			sprite->thinker = std::make_shared<ThinkerLogic>([](Sprite& sprite1, Map& map, float deltaT) {
				sprite1.angle += 100 * deltaT;
				});
		}

		sprites.push_back(sprite);
	}

	/*sprites[2]->thinker = std::make_shared<ThinkerLogic>([](Sprite& sprite, Map& map, float deltaT) {
		sprite.move(map, sf::Vector2f(0, 0.7f) * deltaT);
		});*/

	if (!player) {
		auto def = spriteDef[0];
		auto sprite = std::make_shared<Sprite>(sf::Vector2f{ 2,2 }, def.size, def.indexTexture, 0);
		sprites.push_back(sprite);
		player = std::make_unique<Player>(sprite.get());
	}

	for (auto sp : sprites)
	{
		nowMap->insertInBlockMap((sf::Vector2i)sp->position, sp.get());
		sp->setupBlockmap(*nowMap);
	}
}

void Game::resetMap()
{
	for (auto sp = sprites.begin(); sp != sprites.end();)
	{
		if ((*sp)->texture != -1)
		{
			sp = sprites.erase(sp);
		}
		else
		{
			sp++;
		}
	}

	for (auto sp : nowMap->getMapSprites())
	{
		if (sp.idx != 0)
		{
			auto def = spriteDef[sp.idx];
			auto sprite = std::make_shared<Sprite>(def, sp);
			sprites.push_back(sprite);
		}
	}
	
	for (auto sp : sprites)
	{
		sp->setupBlockmap(*nowMap);
	}
}

void Game::update(float deltaTime)
{
	player->UpdatePlayer(deltaTime, *nowMap, *window);
	for (auto sp : sprites)
	{
		if (sp->thinker)
		{
			sp->thinker->update(*sp, *nowMap, deltaTime);
		}
	}

	bool t = false;
	for (auto sp = sprites.begin(); sp != sprites.end();)
	{
		if ((*sp)->healPoint <= 0.0f)
		{
			nowMap->removeInBlockMap((sf::Vector2i)(*sp)->position, sp->get());
			for (int i = 0; i < sprites.size();i++)
			{
				if (sprites[i] == (*sp))
				{
					sprites.erase(sprites.begin() + i);
					t = true;
					break;
				}
			}
			if (t)
			{
				break;
			}
		}
		else
		{
			sp++;
		}
	}
}

void Game::render()
{
	window->clear();

	renderer.Draw3DView(*window, player->sprite->position,
		player->sprite->angle, *nowMap, sprites);
	player->DrawPlayerUI(*window);

	window->display();
}