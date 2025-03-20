#include "DialogSystem.h"

Dialog::Dialog(sf::RenderWindow* _window, Data* _data) 
{
	window = _window;
	data = _data;
	isActive = false;
	nowKey = 0;
	npc = nullptr;

	sf::RectangleShape nameBase{ {SCREEN_W, 30} };
	nameBase.setFillColor(sf::Color(100, 100, 100));
	nameBase.setPosition({0,0});
	sf::Text nameText(L"", Resources::UIFont, 20);
	shapes.push_back({ nameBase, nameText });

	sf::RectangleShape dataBase{ {SCREEN_W, 180} };
	dataBase.setFillColor(sf::Color(100, 100, 100));
	dataBase.setPosition({ 0, 50 });
	sf::Text dataText(L"", Resources::UIFont, 20);
	shapes.push_back({ dataBase, dataText });
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

	for (auto g : shapes)
	{
		g.text.setString(L"");
	}

	buttons.clear();
}

void Dialog::check()
{
	/*nowKey = npc->npcReac();*/
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
	std::wstring npcText = data->getText(keys[0]).first;

	std::vector<std::pair<std::wstring, int>> variants;
	for (int i = 1; i < keys.size(); i++) { variants.push_back(data->getText(keys[i])); }

	shapes[0].text.setString(npc->spDef.name);
	shapes[0].text.setPosition({ shapes[0].shape.getPosition().x + 5, shapes[0].shape.getPosition().y});

	shapes[1].text.setString(npcText);
	shapes[1].text.setPosition({ shapes[1].shape.getPosition().x + 5, shapes[1].shape.getPosition().y});

	sf::Vector2f pos(0, 245);

	sf::Text text("", Resources::UIFont, 15);
	text.setFillColor(sf::Color::Black);

	sf::RectangleShape rect({ SCREEN_W, 40 });
	rect.setFillColor(sf::Color(50, 50, 50));

	DialogButton b;
	int i = 0;
	buttons.clear();
	for (; i < variants.size(); i++)
	{
		text.setPosition({ pos.x + 5, pos.y + 15 });
		rect.setPosition(pos);
		text.setString(variants[i].first);

		b = DialogButton(rect, text);
		b.setFunc([=]() {nowKey = variants[i].second;});
		buttons.push_back(std::make_shared<DialogButton>(b));

		pos.y += 50;
	}
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
			for (auto b : buttons)
			{
				if (b->isClicked(worldPos))
				{
					b->update();
					check();
					return;
				}
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
	for (auto s : shapes)
	{
		window->draw(s.shape);
		window->draw(s.text);
	}

	for (auto b : buttons)
	{
		b->drawButton(window);
	}
}