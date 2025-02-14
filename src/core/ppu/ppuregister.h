#pragma once

#include "common/pch.h"

struct PPURegister
{
    /*  PPUCTRL
    Address: 0x2000
    Bits: VPHB SINN
    Description:
     - NMI enable (V), 
     - PPU master/slave (P), 
     - Sprite height (H), 
     - Background tile select (B), 
     - Sprite tile select (S), 
     - Increment mode (I), 
     - Nametable select (NN)
     */
    uint8_t ppuctrl;

    /*  PPUMASK
    Address: 0x2001
    Bits: BGRs bMmG
    Description:
     - Color emphasis (BGR),
     - Sprite enable (s),
     - Background enable (b),
     - Sprite left column enable (M),
     - Background left column enable (m),
     - Greyscale (G)
    */
    uint8_t ppumask;

    /* PPUSTATUS

    Address: 0x2002
    Bits: VSO- ----
    Description:
     - Vblank (V),
     - Sprite 0 hit (S),
     - Sprite overflow (O)
    */
    uint8_t ppustatus;

    /* OAMADDR
    Address: 0x2003
    Bits: aaaa aaaa	
    Description: OAM read/write address
    */
    uint8_t oamaddr;

    /* OAMDATA
    Address: 0x2004
    Bits: dddd dddd
    Description: OAM data read/write address
    */
    uint8_t oamdata;


    /* PPUSCROLL
    Address: 0x2005
    Bits: xxxx xxxx
    Description: Fine scroll position
    */
    uint8_t ppuscroll;

    /* PPUADDR
    Address: 0x2006
    Bits: aaaa aaaa
    Description: dddd dddd
    */
    uint8_t ppuaddr;

    /* PPUDATA
    Address: 0x2007
    Bits: dddd dddd
    Description: PPU data read/write
    */
    uint8_t ppudata;

    /* OAMDMA
    Address: 0x4014
    Bits: dddd dddd
    Description: OAM DMA high address.
    */
    uint8_t oamdma;

    /* PPULatch */
    uint8_t ppulatch;
};