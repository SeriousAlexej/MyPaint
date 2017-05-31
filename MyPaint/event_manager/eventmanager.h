#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "../mypaint_api.h"
#include "eventsubscriber.h"

namespace mp
{

/**
 * CEventManager is a singleton class that
 * dispatches received messages to corresponding
 * subscribers.
 */
class MYPAINT_API CEventManager
{
public:
    ~CEventManager() = default;

    static CEventManager& GetInstance();

    void SendEvent(const CEvent& event);
    void SubscribeFor(const std::string& eventName, CEventSubscriber* subscriber);
    void UnsubscribeFrom(const std::string& eventName, CEventSubscriber* subscriber);

private:
    CEventManager() = default;
    CEventManager(const CEventManager&) = delete;
    CEventManager(const CEventManager&&) = delete;
    void operator=(const CEventManager&) = delete;
    void operator=(const CEventManager&&) = delete;

    //hash table, where key is hash from event name
    //and values are sets of pointers to subscribers
    typedef std::unordered_map<std::size_t, std::unordered_set<CEventSubscriber*>> Subscribers_table;

    Subscribers_table subscribers;
};

}

#endif
