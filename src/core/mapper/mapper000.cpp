#include "mapper000.h"

#include <iostream>

uint8_t Mapper000::read(uint16_t addr)
{
    uint8_t byte = 0;

    if (addr < 0x2000u)
        byte = (m_chrbanks == 0u) ? read_chrram(addr) : read_chrrom(addr);
    else if (addr < 0x3F00)
    {
        if (m_mirroring == Mirroring::HORIZONTAL)
        {
            if (addr < 0x2400)
                byte = m_vram[addr - 0x2000];
            else if (addr < 0x2800)
                byte = m_vram[addr - 0x2400];
            else if (addr < 0x2C00)
                byte = m_vram[addr - 0x2400];
            else
                byte = m_vram[addr - 0x2800];
        }
        else if (m_mirroring == Mirroring::VERTICAL)
            byte = m_vram[addr % m_vram.size()];
    }
    else if (0x6000u <= addr && addr < 0x8000u)
        byte = read_prgram(addr - 0x6000u);
    else if (0x8000u <= addr && addr <= 0xFFFFu)
        byte = read_prgrom(addr - 0x8000u);

    return byte;
}

void Mapper000::write(uint8_t byte, uint16_t addr)
{
    if (addr < 0x2000u)
    {
        if (m_chrbanks == 0u)
            write_chrram(byte, addr);
    }
    else if (addr < 0x3F00)
    {
        if (m_mirroring == Mirroring::HORIZONTAL)
        {
            if (addr < 0x2400)
                m_vram[addr - 0x2000] = byte;
            else if (addr < 0x2800)
                m_vram[addr - 0x2400] = byte;
            else if (addr < 0x2C00)
                m_vram[addr - 0x2400] = byte;
            else
                m_vram[addr - 0x2800] = byte;
        }
        else if (m_mirroring == Mirroring::VERTICAL)
            m_vram[addr % m_vram.size()] = byte;
    }
    else if (0x6000u <= addr && addr < 0x8000u)
        write_prgram(byte, addr - 0x6000u);
}