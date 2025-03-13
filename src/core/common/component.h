#pragma once

#include "common/pch.h"

enum class Event
{
    BRK,
    IRQ,
    NMI,
    OAMDMA,
    NONE,
};

class Component
{
public:
    /* Reads a value into byte from internal memory, */
    virtual uint8_t read(uint16_t addr) = 0;

    /* Writes to an internal address */
    virtual void write(uint8_t byte, uint16_t addr) = 0;

    /* Returns the ID of the component */
    virtual std::string_view get_id() const = 0;

    virtual ~Component() = default;

    virtual void process_events() {};

    void push_event(Event event) { m_events.push_back(event); }
    void pop_event() { if (m_events.size()) m_events.pop_back(); }

protected:

    std::vector<Event> m_events;
};