#include "ppu.h"

bool PPU::read(uint8_t &byte, uint16_t addr)
{
    if (addr >= 0x4000u)
        return false;

    switch (addr % 0x8u)
    {
    case 0x0u:
        byte = m_state.r.ppulatch;
        break;
    case 0x1u:
        byte = m_state.r.ppulatch;
        break;
    case 0x2u:
        byte = m_state.r.ppustatus;
        m_state.r.ppulatch = byte;
        break;
    case 0x3u:
        byte = m_state.r.ppulatch;
        break;
    case 0x4u:
        byte = m_state.r.oamdata;
        m_state.r.ppulatch = byte;
        break;
    case 0x5u:
        byte = m_state.r.ppuscroll;
        m_state.r.ppulatch = byte;
        break;
    case 0x6u:
        byte = m_state.r.ppulatch;
        break;
    case 0x7u:
        byte = m_state.r.ppudata;
        m_state.r.ppulatch = byte;
        break;
    default:
        return false;
    }

    return true;
}

bool PPU::write(uint8_t byte, uint16_t addr)
{
    /* Writes to any PPU port will also fill the bits in the latch */

    if (addr >= 0x4000u)
        return false;
    
    switch (addr % 0x8u)
    {
    case 0x0u:
        m_state.r.ppuctrl = byte;
        break;
    case 0x1u:
        m_state.r.ppumask = byte;
        break;
    case 0x2u:
        break;
    case 0x3u:
        m_state.r.oamaddr = byte;
        break;
    case 0x4u:
        m_state.r.oamdata = byte;
        break;
    case 0x5u:
        m_state.r.ppuscroll = byte;
        break;
    case 0x6u:
        m_state.r.ppuaddr = byte;
        break;
    case 0x7u:
        m_state.r.ppudata = byte;
        break;
    default:
        return false;
    }

    m_state.r.ppulatch = byte;
    return true;
}

void PPU::broadcast(Event event)
{

}

void PPU::service(Event event)
{

}

void PPU::step()
{

}