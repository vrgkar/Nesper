#include "cpu.h"

CPU::CPU()
    : m_active(false), m_opcode(nullptr), m_mode(nullptr), m_state(std::make_shared<ReadyState>()), m_r(), m_cycles(7)
{
    m_active = true;
}

CPU::CPU(Bus *bus)
    : m_active(false), m_state(std::make_shared<ReadyState>()), m_r(), m_bus(bus)
{
    m_active = true;
}

bool CPU::fetch(uint8_t &byte, uint16_t addr)
{
    return true;
}

bool CPU::commit(uint8_t byte, uint16_t addr)
{
    return true;
}

void CPU::step()
{
    execute_state();
    tick();   
}

void CPU::poll_interrupts()
{}