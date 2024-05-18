#include "apu.h"

bool APU::read(uint8_t &byte, uint16_t addr)
{
    if (0x4000u <= addr && addr < 0x4020u)
        ;
    else
        return false;

    return true;
}

bool APU::write(uint8_t byte, uint16_t addr)
{
    if (0x4000u <= addr && addr < 0x4020u)
        ;
    else
        return false;

    return true;
}

void APU::broadcast(Event event)
{
    
}

void APU::service(Event event)
{
    
}