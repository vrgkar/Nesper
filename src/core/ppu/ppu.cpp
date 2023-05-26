#include "ppu.h"

bool PPU::fetch(uint8_t &byte, uint16_t addr)
{
    if (0x2000u <= addr && addr < 0x4000u)
        ;
    else
        return false;

    return true;
}

bool PPU::commit(uint8_t byte, uint16_t addr)
{
    if (0x2000u <= addr && addr < 0x4000u)
        ;
    else
        return false;

    return true;
}