#pragma once

#include <cstdint>

class CPU;
class RAM;
class PPU;
class APU;
class Mapper;

class Bus
{
public:

    uint8_t cpu_read(uint16_t addr);
    void cpu_write(uint8_t byte, uint16_t addr);

    uint8_t ppu_read(uint16_t addr);
    void ppu_write(uint8_t byte, uint16_t addr);

    void connect(CPU *cpu) { m_cpu = cpu; }
    void connect(RAM *ram) { m_ram = ram; }
    void connect(PPU *ppu) { m_ppu = ppu; }
    void connect(APU *apu) { m_apu = apu; }
    void connect(Mapper *map) { m_map = map; }

    
private:
    CPU *m_cpu = nullptr;
    RAM *m_ram = nullptr;
    PPU *m_ppu = nullptr;
    APU *m_apu = nullptr;
    Mapper *m_map = nullptr;
};
