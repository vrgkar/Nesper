#pragma once

#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "apu/apu.h"
#include "ram/ram.h"

class NES
{
public:
    void execute();

private:
    Bus m_bus;

    CPU m_cpu;
    PPU m_ppu;
    APU m_apu;
    RAM m_ram;
};

