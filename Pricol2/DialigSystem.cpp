#include "DialogSystem.h"

Dialog::Dialog(sf::RenderWindow* _window, Data* _data, UIManager* _uiManager)
{
	window = _window;
	data = _data;
	nowKey = 0;
	isActive = false;
	npc = nullptr;

	uiManager = _uiManager;
}

void Dialog::setTrade(Trade* _trade)
{
	trade = _trade;
}

void Dialog::start(Npc* _npc)
{
	window->setMouseCursorVisible(true);
	isActive = true;
	npc = _npc;
	nowKey = _npc->npcDefData.startKey;
	check();
}

void Dialog::stop()
{
	window->setMouseCursorVisible(false);
	isActive = false;
	npc = nullptr;
	uiManager->deleteNow();
}

void Dialog::check()
{
	if (trade->isActive) { return; }

	if (nowKey == 0)
	{
		stop();
		return;
	}
	else
	{
		if (nowKey >= 400)
		{
			trade->start(nowKey);
			nowKey = 0;
			return;
		}
	}
	init();
}

void Dialog::init()
{
	auto keys = data->getKeys(nowKey);

	std::map<int, std::wstring, std::greater<int>> variants;
	for (int i = 0; i < keys.size(); i++) 
	{ 
		auto d = data->getText(keys[i]);
		variants[d.second] = d.first; 
	}

	uiManager->initDialog(variants, npc->spDef.name);
}

void Dialog::update()
{
	if (trade->isActive)
	{
		trade->update();
		check();
		return;
	}

	if (window->hasFocus())
	{
		static bool flag = false;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !flag)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			sf::Vector2i worldPos = (sf::Vector2i)window->mapPixelToCoords(mousePos);

			flag = true;
			int key = uiManager->checkButton(worldPos);
			if (key != -1) 
			{
				nowKey = key;
				check();
			}
		}
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			flag = false;
		}
	}
}

void Dialog::draw()
{
	uiManager->drawNow();
}