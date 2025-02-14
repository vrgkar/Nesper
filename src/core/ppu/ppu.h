#pragma once

#include "common/pch.h"
#include "common/component.h"

#include "bus/bus.h"

#include "ppustate.h"

class PPU final : public Component
{
public:
    bool read(uint8_t &byte, uint16_t addr) override;
    bool write(uint8_t byte, uint16_t addr) override;

    void broadcast(Event event) override;
    void service(Event event) override;

    void step();

    void set_bus(Bus *bus) { m_bus = bus; }

    std::string_view get_id() const override { return "PPU"; }

private:

    /* Common Bus */
    Bus *m_bus = nullptr;

    /* Internal PPU State */
    PPUState m_state;

    /* Internal OAM Memory */
    std::array<uint8_t, 64*4> m_oam;
};