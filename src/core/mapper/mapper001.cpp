#include "mapper001.h"

uint8_t Mapper001::read(uint16_t addr)
{
    uint8_t byte = 0;
    if (addr < 0x2000u)
        byte = (m_chrbanks == 0u) ? read_chrram(addr) : read_chrrom(addr);
    else if (0x6000u <= addr && addr < 0x8000u)
        byte = read_prgram(addr - 0x6000u);
    else if (0x8000u <= addr && addr <= 0xFFFFu)
        byte = read_prgrom(addr - 0x8000u);

    return byte;
}

void Mapper001::write(uint8_t byte, uint16_t addr)
{
    if (addr < 0x2000u && m_chrbanks == 0u)
        write_chrram(byte, addr);
    else if (0x8000u <= addr && addr <= 0xFFFFu)
        ;
}