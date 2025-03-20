#include "DialogSystem.h"

Dialog::Dialog(sf::RenderWindow* _window, Data* _data) 
{
	window = _window;
	data = _data;
	nowKey = 0;
	isActive = false;
	npc = nullptr;

	uiManager = new UIManager(window);
}

void Dialog::start(Npc* _npc)
{
	window->setMouseCursorVisible(true);
	isActive = true;
	npc = _npc;
	nowKey = _npc->startKey;
	check();
}

void Dialog::stop()
{
	window->setMouseCursorVisible(false);
	isActive = false;
	npc = nullptr;
	uiManager->deleteDialog();
}

void Dialog::check()
{
	nowKey = npc->npcReac(nowKey);
	if (nowKey == 0)
	{
		stop();
		return;
	}
	else
	{
		init();
	}
}

void Dialog::init()
{
	auto keys = data->getKeys(nowKey);

	std::vector<std::pair<std::wstring, int>> variants;
	for (int i = 0; i < keys.size(); i++) { variants.push_back(data->getText(keys[i])); }

	uiManager->initDialog(variants, npc->spDef.name);
}

void Dialog::update()
{
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
				return;
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
	uiManager->drawDialog();
}