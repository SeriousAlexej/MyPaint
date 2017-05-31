#ifndef EVENTS_H
#define EVENTS_H
#include <string>
#include "../mypaint_api.h"

namespace mp
{

/**
 * CEvent is a class that contains information
 * about some default event (mouse/keyboard events)
 * or is a user-defined event (for sending messages/notifications etc)
 */
class MYPAINT_API CEvent
{
public:
    enum EEventType
    {
        ET_USER = 0,
        ET_MOUSE_PRESS,
        ET_MOUSE_RELEASE,
        ET_MOUSE_MOVE //can extend with key events
    };

    enum EMouseButton
    {
        MB_L = 0,
        MB_R
    };

    explicit CEvent(const std::string& name) :
        name(name)
    {}

    std::string  name;
    EEventType   type = ET_USER;
    EMouseButton button = MB_L;
    int          mouseX = 0;
    int          mouseY = 0;
};

}

#endif
