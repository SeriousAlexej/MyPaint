#include "eventmanager.h"

namespace mp
{

CEventManager& CEventManager::GetInstance()
{
    static CEventManager instance;
    return instance;
}

void CEventManager::SubscribeFor(const std::string& eventName, CEventSubscriber* subscriber)
{
    const std::size_t eventNameHash = std::hash<std::string>()(eventName);
    subscribers[eventNameHash].insert(subscriber);
}

void CEventManager::UnsubscribeFrom(const std::string& eventName, CEventSubscriber* subscriber)
{
    const std::size_t eventNameHash = std::hash<std::string>()(eventName);
    if(subscribers.find(eventNameHash) != subscribers.end())
    {
        auto& subscrSet = subscribers[eventNameHash];
        auto foundPos = subscrSet.find(subscriber);
        if(foundPos != subscrSet.end())
        {
            subscrSet.erase(foundPos);
        }
    }
}

void CEventManager::SendEvent(const CEvent &event)
{
    const std::size_t eventNameHash = std::hash<std::string>()(event.name);
    if(subscribers.find(eventNameHash) != subscribers.end())
    {
        auto& subscrSet = subscribers[eventNameHash];

        for(CEventSubscriber* es : subscrSet)
        {
            es->ReceiveEvent(event);
        }
    }
}

}
