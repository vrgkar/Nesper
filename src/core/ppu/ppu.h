#pragma once

#include <array>

#include "../system/component/component.h"
#include "ppuregister/ppuregister.h"

#include "../bus/bus.h"

class PPU final : public Component
{
public:
    bool fetch(uint8_t &byte, uint16_t addr) override;
    bool commit(uint8_t byte, uint16_t addr) override;

    void step();

    std::string_view get_id() override { return "PPU"; }

private:

    /* Common Bus */
    Bus *bus = nullptr;

    /* Internal PPU Registers */
    PPURegister m_r;

    /* Internal OAM Memory */
    std::array<uint8_t, 64*4> m_oam;
};