#include "bus.h"

#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "apu/apu.h"
#include "ram/ram.h"
#include "mapper/mapper.h"

uint8_t Bus::cpu_read(uint16_t addr)
{
    uint8_t byte = 0;

    if (m_ram && m_ram->read(byte, addr))
        return byte;
    else if (m_ppu && m_ppu->read(byte, addr))
        return byte;
    else if (m_apu && m_apu->read(byte, addr))
        return byte;
    else if (m_map && m_map->read(byte, addr))
        return byte;

    return byte;
}

void Bus::cpu_write(uint8_t byte, uint16_t addr)
{
    if (m_ram && m_ram->write(byte, addr))
        return;
    else if (m_ppu && m_ppu->write(byte, addr))
        return;
    else if (m_apu && m_apu->write(byte, addr))
        return;
    else if (m_map && m_map->write(byte, addr))
        return;
}