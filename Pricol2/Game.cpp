#include "Game.h"
#include <Windows.h>

Game::Game(sf::RenderWindow* _window, Map* _nowMap) :
	renderer(), window{ _window }, nowMap{ _nowMap }
{
	screenMidlePos = { (int)(SCREEN_W / 2), (int)(SCREEN_H / 2) };
	for (auto sp : nowMap->getMapSprites())
	{
		auto def = spriteDef[sp.idx];
		auto sprite = std::make_shared<Sprite>(def, sp);

		if (sp.idx == 0) {
			player = std::make_unique<Player>(Player(sprite.get(), _nowMap));
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
		player = std::make_unique<Player>(Player(sprite.get(), _nowMap));
	}

	for (auto sp : sprites)
	{
		nowMap->insertInBlockMap((sf::Vector2i)sp->position, sp.get());
		sp->setupBlockmap(*nowMap);
	}
}

void Game::getInput(sf::Event event, float deltaTime)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{			
		player->fire();
	}
	if (event.type == sf::Event::MouseWheelMoved)
	{
		bool flag = event.mouseWheelScroll.delta > 0 ? true : false;
		player->swapGun(flag);
	}
}

void Game::getInput(float deltaTime)
{
	float radiansAngle = player->sprite->angle * PI / 180.0f;
	sf::Vector2f verticalMoveParametrs(cos(radiansAngle), sin(radiansAngle));
	sf::Vector2f horizontalMoveParametrs(-verticalMoveParametrs.y, verticalMoveParametrs.x);
	sf::Vector2f deltaPos(0, 0);
	bool lShiftPressed = false;
	float deltaX = 0, deltaY = 0;
	if (window->hasFocus())
	{
		if (GetAsyncKeyState('A'))
		{
			deltaPos -= horizontalMoveParametrs;
		}
		else if (GetAsyncKeyState('D'))
		{
			deltaPos += horizontalMoveParametrs;
		}
		if (GetAsyncKeyState('W'))
		{
			deltaPos += verticalMoveParametrs;
		}
		else if (GetAsyncKeyState('S'))
		{
			deltaPos -= verticalMoveParametrs;
		}
		if (GetAsyncKeyState('R'))
		{
			player->reloadingGun();
		}
		if (GetAsyncKeyState(VK_LSHIFT))
		{
			lShiftPressed = true;
		}
		if (GetAsyncKeyState(VK_SPACE))
		{
			player->jump();
		}
		if (GetAsyncKeyState('E'))
		{
			player->use();
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		deltaX = (mousePos.x - screenMidlePos.x) / 2.0f;
		deltaY = (mousePos.y - screenMidlePos.y) / 2.0f;
		sf::Mouse::setPosition(screenMidlePos, *window);
	}

	player->checkBoost(lShiftPressed, deltaTime);
	player->move(deltaPos * deltaTime);
	player->updateMouseData({deltaX, deltaY}, deltaTime);
}

void Game::resetMap(Map* newMap)
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

void Game::makeCycle(float deltaTime)
{
//#if !DEBUG
//	if (deltaTime > 0.01)
//	{
//		deltaTime = 0;
//	}
//#endif //DEBUG

	getInput(deltaTime);
	update(deltaTime);
	render();
}

void Game::drawAim()
{
	sf::CircleShape aim{};
	aim.setRadius(1.0f);
	aim.setFillColor(sf::Color::Black);
	aim.setPointCount(16);
	aim.setPosition((sf::Vector2f)screenMidlePos);

}

void Game::render()
{
	window->clear();
	renderer.Draw3DView(*window, player.get(), sprites);
	player->DrawPlayerUI(*window);
	sf::CircleShape aim{};
	aim.setRadius(2.5f);
	aim.setOrigin({ aim.getRadius(),aim.getRadius() });
	aim.setFillColor(sf::Color::Black);
	aim.setPointCount(16);
	aim.setPosition((sf::Vector2f)screenMidlePos);
	window->draw(aim);
	window->display();
}
