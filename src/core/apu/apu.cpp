#include "apu.h"

bool APU::fetch(uint8_t &byte, uint16_t addr)
{
    if (0x4000u <= addr && addr < 0x4020u)
        ;
    else
        return false;

    return true;
}

bool APU::commit(uint8_t byte, uint16_t addr)
{
    if (0x4000u <= addr && addr < 0x4020u)
        ;
    else
        return false;

    return true;
}