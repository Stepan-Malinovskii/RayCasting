#include "DialogSystem.h"

Dialog::Dialog(sf::RenderWindow* _window, UIManager* _uiManager,
	ItemManager* _weaponManager) : window{ _window },
	uiManager{ _uiManager }, weaponManager{ _weaponManager },
	npc{ nullptr }, player{ nullptr }, 
	dialogState{ [=](float deltaTime) { update(); }, [=]() { draw();}} {}
void Dialog::setPlayer(Player* _player) { player = _player; }

void Dialog::start(Npc* _npc)
{
	auto& event = EventSystem::getInstance();
	event.trigger<RenderState*>("SWAP_STATE", &dialogState);
	window->setMouseCursorVisible(true);
	npc = _npc;
	npc->setEndFunc([=]() {stop();});
	npc->init();
}

void Dialog::stop()
{
	window->setMouseCursorVisible(false);
	npc = nullptr;
	auto& event = EventSystem::getInstance();
	event.trigger<RenderState*>("SWAP_STATE", nullptr);
}

//void Dialog::buy()
//{
//	/*if (!choose) return;
//	
//	if (choose->cost > player->money) return;
//	
//	player->money -= choose->cost;
//
//	player->takeItem(choose);
//	choose = nullptr;*/
//
//	npc->use();
//}

//void Dialog::init()
//{
//	npc->init();
//}

//void Dialog::initDialog()
//{
//	auto& data = Data::getInstance();
//	auto keys = data.getKeys(nowKey);
//
//	std::map<int, std::wstring, std::greater<int>> variants;
//
//	for (int i = 0; i < keys.size(); i++)
//	{
//		auto d = data.getText(keys[i]);
//		variants[d.second] = d.first;
//	}
//
//	uiManager->initDialog(variants, name);
//}

//void Dialog::initTrade()
//{
//	title.clear();
//
//	if (startKey == 400)
//	{
//		int i = 3;
//
//		auto item1 = weaponManager->getGuns();
//
//		for (; i < item1.size(); i++)
//		{
//			title[i] = (Itemble*)item1[i];
//		}
//
//		auto item2 = weaponManager->getImprovs();
//
//		for (; i < item2.size() + item1.size(); i++)
//		{
//			title[i] = (Itemble*)item2[i - item1.size()];
//		}
//	}
//	else if (startKey == 401)
//	{
//		auto item = weaponManager->getItems();
//
//		for (int i = 0; i < item.size(); i++)
//		{
//			title[i] = (Itemble*)item[i];
//		}
//	}
//	else if (startKey == 402)
//	{
//		auto item = weaponManager->getTravelItem();
//
//		for (int i = 0; i < item.size(); i++)
//		{
//			title[i] = item[i];
//		}
//	}
//
//	uiManager->initTrade(title, player);
//}

//void Dialog::check()
//{
//	
//}

//void Dialog::checkDialog()
//{
//	if (nowKey == 0)
//	{
//		stop();
//	}
//	else if (nowKey >= 400)
//	{
//		if (nowKey == 777)
//		{
//			stop();
//			auto& event = EventSystem::getInstance();
//			event.trigger<int>("SWAPLOC", BASE_N);
//			return;
//		}
//
//		isTrade = true;
//		startKey = nowKey;
//		nowKey = 0;
//		start(startKey);
//	}
//	else
//	{
//		init();
//	}
//}

//void Dialog::checkTrade()
//{
//	if (nowKey == -100)
//	{
//		stop();
//	}
//	else if (nowKey == -200)
//	{
//		buy();
//	}
//	else
//	{
//		choose = title[nowKey];
//	}
//}

void Dialog::update()
{
	if (!window->hasFocus()) return;
	
	static bool isMouseDown = false;
	bool isPress = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	if (isPress && !isMouseDown)
	{
		int key = uiManager->checkButton();

		if (key != -1)
		{
			npc->update(key);
		}

		isMouseDown = true;
	}
	if (!isPress)
	{
		isMouseDown = false;
	}
}

void Dialog::draw()
{
	uiManager->drawNow();
}