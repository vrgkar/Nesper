#pragma once

#include <cstdint>
#include <string_view>

#include "common/event.h"

class Component
{
public:
    /* Reads a value into byte from internal memory,
       and returns whether byte was read or not */
    virtual bool read(uint8_t &byte, uint16_t addr) = 0;

    /* Writes to an internal address, 
       and returns whether the value was written to or not */
    virtual bool write(uint8_t byte, uint16_t addr) = 0;

    /* Returns the ID of the component */
    virtual std::string_view get_id() const = 0;

    virtual void broadcast(Event event) = 0;
    virtual void service(Event event) = 0;

    virtual ~Component() = default; 

};