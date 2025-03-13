#pragma once

#include "common/pch.h"
#include "common/component.h"

class CPU;
class PPU;
class APU;
class Mapper;

class Bus
{
public:

    /* Read from the CPU bus */
    uint8_t read_cpu(uint16_t addr);

    /* Write to the CPU bus */
    void write_cpu(uint8_t byte, uint16_t addr);

    /* Read from the PPU bus */
    uint8_t read_ppu(uint16_t addr);

    /* Write to the PPU bus */
    void write_ppu(uint8_t byte, uint16_t addr);

    void broadcast(void *component, Event event);

    void step();

    void connect(CPU *cpu) { m_cpu = cpu; }
    void connect(PPU *ppu) { m_ppu = ppu; }
    void connect(APU *apu) { m_apu = apu; }
    void connect(Mapper *mapper) { m_mapper = mapper; }
    
private:
    
    /* Components */
    CPU *m_cpu = nullptr;
    PPU *m_ppu = nullptr;
    APU *m_apu = nullptr;
    Mapper *m_mapper = nullptr;

    uint8_t m_mdr = 0;
    uint16_t m_mar = 0;

    /* Internal RAM */
    std::array<uint8_t, 2048> m_ram = {};

    enum class State
    {
        IDLE,
        OAMDMA,
        DMCDMA,
    } m_state = State::IDLE;

    std::vector<Event> m_events;

private:
    void process_events();
    bool OAMDMA();
};
