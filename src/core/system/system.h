#pragma once

#include <queue>

#include "component/component.h"
#include "event/event.h"

class System
{
public:

    void receive(Component *comp, Event event) { m_events.push(std::make_pair(event, comp)); }
    virtual void service() = 0;

    virtual ~System() = default;

protected:
    std::priority_queue<std::pair<Event, Component*>> m_events;

};