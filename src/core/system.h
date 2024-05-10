#pragma once

#include <queue>

#include "component.h"
#include "common/event.h"

class System
{
public:

    virtual void receive(Component *comp, Event event) { m_events.push(std::make_pair(event, comp)); }
    virtual void service() {};

    virtual ~System() = default;

protected:
    std::priority_queue<std::pair<Event, Component*>> m_events;

};