#pragma once

#include <cstdint>
#include <string_view>

#include "common/event.h"

class Component
{
public:
    /* Fetches a value into byte from internal memory,
       and returns whether byte was fetched or not */

    virtual bool fetch(uint8_t &byte, uint16_t addr) = 0;

    /* Commits to an internal address, 
       and returns whether the value was committed or not */
    virtual bool commit(uint8_t byte, uint16_t addr) = 0;

    /* Returns the ID of the component */
    virtual std::string_view get_id() = 0;

    virtual void broadcast(Event event);
    virtual void service(Event event);

    virtual ~Component() = default; 

};