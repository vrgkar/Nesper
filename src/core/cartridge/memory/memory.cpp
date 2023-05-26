#include "memory.h"

void Memory::load(const ROM &rom)
{
    m_prgrom = rom.prgrom;
    m_chrrom = rom.chrrom;

    m_prgram.resize(0x2000);
    m_chrram.resize(0x2000);
}

uint8_t Memory::read_prgram(uint16_t addr) const
{
    return m_prgram[addr % prgram_size()];
}

uint8_t Memory::read_prgrom(uint16_t addr) const
{
    return m_prgrom[addr % prgrom_size()];
}

uint8_t Memory::read_chrram(uint16_t addr) const
{
    return m_chrram[addr % chrram_size()];
}

uint8_t Memory::read_chrrom(uint16_t addr) const
{
    return m_chrrom[addr % chrrom_size()];
}