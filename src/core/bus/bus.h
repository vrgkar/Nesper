#pragma once

#include <cstdint>

class CPU;
class RAM;
class PPU;
class APU;
class Cartridge;

class Bus
{
public:

    uint8_t read(const CPU &cpu, uint16_t addr);
    void write(const CPU &cpu, uint8_t byte, uint16_t addr);

    uint8_t read(const PPU &ppu, uint16_t addr);
    void write(const PPU &ppu, uint8_t byte, uint16_t addr);

    void connect(CPU *cpu) { m_cpu = cpu; }
    void connect(RAM *ram) { m_ram = ram; }
    void connect(PPU *ppu) { m_ppu = ppu; }
    void connect(APU *apu) { m_apu = apu; }
    void connect(Cartridge *cart) { m_cart = cart; }

    
private:
    CPU *m_cpu = nullptr;
    RAM *m_ram = nullptr;
    PPU *m_ppu = nullptr;
    APU *m_apu = nullptr;
    Cartridge *m_cart = nullptr;
};
