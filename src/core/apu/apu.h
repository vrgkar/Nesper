#pragma once

#include "common/component.h"
#include "bus/bus.h"

class APU final : public Component
{
public:
    uint8_t read(uint16_t addr) override;
    void    write(uint8_t byte, uint16_t addr) override;

    void connect(Bus *bus) { m_bus = bus; m_bus->connect(this); }

    std::string_view get_id() const override { return "APU"; }

private:
    Bus *m_bus = nullptr;
};