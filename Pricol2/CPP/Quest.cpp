#include "Quest.h"
#include "DataBase.h"

Quest::Quest(QuestData data) : data{ data } {}

void Quest::updateProgress(int value) { data.progress = std::min(data.progress + value, data.target); }

bool Quest::isCompleted() { return data.progress >= data.target; }

QuestManager::QuestManager()
{
    auto& dataBase = Data::getInstance();
    auto questData = dataBase.getQuest();

    for (size_t i = 0; i < questData.size(); i++)
    {
        addQuest(questData[i]);
    }
}

QuestManager::~QuestManager()
{
    std::vector<QuestData> questData;
    for (auto q : quests)
    {
        questData.push_back(q->data); 
    }

    auto& dataBase = Data::getInstance();
    dataBase.saveQuest(questData);
}

void QuestManager::addQuest(QuestData data) { quests.push_back(new Quest(data)); }

void QuestManager::updateQuests(QuestType type, int value)
{
    for (auto quest : quests) 
    {
        if (quest->data.type == type)
        {
            quest->updateProgress(value);
        }
    }
}

void QuestManager::deleteQuest(Quest* quest) 
{
    for (size_t i = 0; i < quests.size(); i++)
    {
        if (quests[i] == quest)
        {
            quests.erase(quests.begin() + i);
            break;
        }
    }
}

