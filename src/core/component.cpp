#include "component.h"
#include "system.h"

void Component::broadcast(Event event)
{
    system->receive(this, event);
}

void Component::service(Event event)
{
    
}