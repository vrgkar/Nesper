#pragma once

#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "apu/apu.h"
#include "ram/ram.h"
#include "cartridge/cartridge.h"
#include "system/system.h"

class NES : public System
{
public:
    void execute();

    virtual void service() override;

private:
    Bus m_bus;

    CPU m_cpu;
    PPU m_ppu;
    APU m_apu;
    RAM m_ram;
    Cartridge m_cart;
};