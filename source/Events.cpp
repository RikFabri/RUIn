#include "Events.h"

RUIN::Event RUIN::Event::ScrollEvent(float scrollAmount)
{
    Event e{};
    e.m_Type = EventType::MouseScroll;
    e.m_MouseScrollData.scrollAmount = scrollAmount;

    return e;
}

RUIN::Event RUIN::Event::MouseMovedEvent()
{
    Event e{};
    e.m_Type = EventType::MouseMoved;
    
    return e;
}

RUIN::Event RUIN::Event::MouseDownEvent()
{
    Event e{};
    e.m_Type = EventType::MouseDown;
    
    return e;
}

RUIN::Event RUIN::Event::MouseUpEvent()
{
    Event e{};
    e.m_Type = EventType::MouseUp;
    
    return e;
}
