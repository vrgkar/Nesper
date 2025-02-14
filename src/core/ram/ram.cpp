#include "ram.h"

bool RAM::read(uint8_t &byte, uint16_t addr)
{
    if (addr < 0x2000u)
        byte = m_ram[addr % m_ram.size()];
    else
        return false;

    return true;
}

bool RAM::write(uint8_t byte, uint16_t addr)
{
    if (addr < 0x2000u)
        m_ram[addr % m_ram.size()] = byte;
    else
        return false;
    
    return true;
}

void RAM::broadcast(Event event)
{

}

void RAM::service(Event event)
{

}
