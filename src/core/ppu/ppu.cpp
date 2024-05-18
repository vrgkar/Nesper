#include "ppu.h"

bool PPU::read(uint8_t &byte, uint16_t addr)
{
    /*
    Reading any readable PPU port fills the PPU Latch
    Reading from a nominally write-only PPU port returns the current value of the latch.
    */
    if (addr == 0x4014u)
    {
        byte = m_r.ppulatch;
        return true;
    }

    switch (addr % 0x8u)
    {
    case 0x0u:
        byte = m_r.ppulatch;
        break;
    case 0x1u:
        byte = m_r.ppulatch;
        break;
    case 0x2u:
        byte = m_r.ppustatus;
        m_r.ppulatch = byte;
        break;
    case 0x3u:
        byte = m_r.ppulatch;
        break;
    case 0x4u:
        byte = m_r.oamdata;
        m_r.ppulatch = byte;
        break;
    case 0x5u:
        byte = m_r.ppuscroll;
        m_r.ppulatch = byte;
        break;
    case 0x6u:
        byte = m_r.ppulatch;
        break;
    case 0x7u:
        byte = m_r.ppudata;
        m_r.ppulatch = byte;
        break;
    default:
        return false;
    }

    return true;
}

bool PPU::write(uint8_t byte, uint16_t addr)
{
    /* Writes to any PPU port will also fill the bits in the latch */

    if (addr == 0x4014u)
    {
        m_r.oamdma = byte;
        m_r.ppulatch = byte;

        // Broadcast OAMDMA 

        return true;
    }

    switch (addr % 0x8u)
    {
    case 0x0u:
        m_r.ppuctrl = byte;
        break;
    case 0x1u:
        m_r.ppumask = byte;
        break;
    case 0x2u:
        break;
    case 0x3u:
        m_r.oamaddr = byte;
        break;
    case 0x4u:
        m_r.oamdata = byte;
        break;
    case 0x5u:
        m_r.ppuscroll = byte;
        break;
    case 0x6u:
        m_r.ppuaddr = byte;
        break;
    case 0x7u:
        m_r.ppudata = byte;
        break;
    default:
        return false;
    }

    m_r.ppulatch = byte;
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