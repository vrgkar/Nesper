#include "mapper000.h"

bool Mapper000::read(Memory &memory, uint8_t &byte, uint16_t addr)
{
    if (addr < 0x2000u)
        byte = (memory.chr_banks() == 0u) ? memory.read_chrram(addr) : memory.read_chrrom(addr);
    else if (0x6000u <= addr && addr < 0x8000u)
        byte = memory.read_prgram(addr - 0x6000u);
    else if (0x8000u <= addr && addr <= 0xFFFFu)
        byte = memory.read_prgrom(addr - 0x8000u);
    else
        return false;

    return true;
}

bool Mapper000::write(Memory &memory, uint8_t byte, uint16_t addr)
{
    if (addr < 0x2000u && memory.chr_banks() == 0u)
        memory.write_chrram(byte, addr);
    else
        return false;

    return true;
}