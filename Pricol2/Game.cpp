#include "Game.h"

Game::Game(sf::RenderWindow* _window, Map* _nowMap) :
	renderer(), window{ _window }, nowMap{ _nowMap }
{
	for (auto sp : _nowMap->getSprites())
	{
		auto def = spriteDef[sp.idx];
		auto sprite = std::make_shared<Sprite>(sp.position, def.size, def.indexTexture, sp.angle);
		sprites.push_back(sprite);

		if (sp.idx == 0) {
			player = std::make_unique<Player>(sprite.get());
		}
	}

	if (!player) {
		auto def = spriteDef[0];
		auto sprite = std::make_shared<Sprite>(sf::Vector2f{2,2}, def.size, def.indexTexture, 0);
		sprites.push_back(sprite);
		player = std::make_unique<Player>(sprite.get());
	}

	/*sprites[2]->thinker = std::make_shared<ThinkerLogic>(
		[](Sprite& sprite, Map& map, float deltaT) {
			sprite.move(map, sf::Vector2f(0.0f, 0.1f) * deltaT);
		}
	);*/
	for (auto sp : sprites)
	{
		nowMap->insertInBlockMap((sf::Vector2i)sp->position,sp.get());
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
}

void Game::render()
{
	window->clear();

	renderer.Draw3DView(*window, player->sprite->position,
		player->sprite->angle, *nowMap, sprites);
	player->DrawPlayerUI(*window);

	window->display();
}