#include "bus.h"

#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "apu/apu.h"
#include "ram/ram.h"
#include "cartridge/cartridge.h"

uint8_t Bus::read(const CPU &cpu, uint16_t addr)
{
    uint8_t byte = 0;

    if (m_ram && m_ram->fetch(byte, addr))
        return byte;
    else if (m_ppu && m_ppu->fetch(byte, addr))
        return byte;
    else if (m_apu && m_apu->fetch(byte, addr))
        return byte;
    else if (m_cart && m_cart->fetch(byte, addr))
        return byte;

    return byte;
}

void Bus::write(const CPU &cpu, uint8_t byte, uint16_t addr)
{
    if (m_ram && m_ram->commit(byte, addr))
        return;
    else if (m_ppu && m_ppu->commit(byte, addr))
        return;
    else if (m_apu && m_apu->commit(byte, addr))
        return;
    else if (m_cart && m_cart->commit(byte, addr))
        return;
}