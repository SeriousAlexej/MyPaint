#ifndef EVENT_SUBSCRIBER_H
#define EVENT_SUBSCRIBER_H
#include "events.h"

namespace mp
{

/**
 * CEventSubscriber is an interface for receiving
 * events from CEventManager
 */
class MYPAINT_API CEventSubscriber
{
public:
    CEventSubscriber() = default;
    virtual ~CEventSubscriber() = default;

    virtual void ReceiveEvent(const CEvent& event) = 0;
};

}

#endif
