#include "mapper.h"

void Mapper::broadcast(Event event)
{

}

void Mapper::service(Event event)
{

}

void Mapper::load_rom(const ROM &rom)
{
    m_prgrom = rom.prgrom;
    m_chrrom = rom.chrrom;

    m_prgram.resize(0x2000);
    m_chrram.resize(0x2000);
}

uint8_t Mapper::read_prgram(uint16_t addr) const
{
    return m_prgram[addr % m_prgram.size()];
}

uint8_t Mapper::read_prgrom(uint16_t addr) const
{
    return m_prgrom[addr % m_prgrom.size()];
}

uint8_t Mapper::read_chrram(uint16_t addr) const
{
    return m_chrram[addr % m_chrram.size()];
}

uint8_t Mapper::read_chrrom(uint16_t addr) const
{
    return m_chrrom[addr % m_chrrom.size()];
}

void Mapper::write_prgram(uint8_t byte, uint16_t addr)
{
    m_prgram[addr % m_prgrom.size()] = byte;
}

void Mapper::write_chrram(uint8_t byte, uint16_t addr)
{ 
    m_chrram[addr % m_chrrom.size()] = byte;
}