#include "cpu.h"

bool CPU::read(uint8_t &byte, uint16_t addr)
{
    return false;
}

bool CPU::write(uint8_t byte, uint16_t addr)
{
    return false;
}

void CPU::service(Event event)
{
    
}

void CPU::broadcast(Event event)
{

}

std::string CPU::disassemble(uint16_t addr)
{
    auto pstate = m_state;

   // m_state.r.pc = addr;

    m_state = pstate;

    return "";
}

void CPU::step()
{
    ++m_cycles;
    
    if (m_state == State::FETCH)
        fetch();

    if (m_state == State::DECODE)
        decode();

    if (m_state == State::EXECUTE_MODE)
        execute_mode();

    if (m_state == State::EXECUTE_INSTRUCTION)
        execute_instruction();

    if (m_state == State::INTERRUPT)
        interrupt();
}

void CPU::step_instruction()
{
    do
        step();
    while (m_state != State::FETCH);
}

std::string CPU::get_instr_disassembly()
{
    return "";
}