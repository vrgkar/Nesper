#pragma once

#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "apu/apu.h"
#include "ram/ram.h"
#include "mapper/mapper.h"

class NES
{
public:
    NES() = delete;
    void operator=(const NES&) = delete;

private:

    static CPU m_cpu;
    static PPU m_ppu;
    static APU m_apu;
    static RAM m_ram;
    static Mapper m_map;

    static Bus m_bus;
};