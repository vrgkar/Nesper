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

    m_state.r.pc = addr;

    m_state = pstate;

    return "";
}

void CPU::step()
{
    if (m_state.process == CPUProcess::FETCH)
        fetch();

    else if (m_state.process == CPUProcess::DECODE)
        decode();

    else if (m_state.process == CPUProcess::EXECUTE)
        execute();

    else if (m_state.process == CPUProcess::INTERRUPT)
        interrupt();
}

void CPU::step_instruction()
{
    do
        step();
    while (m_state.process != CPUProcess::FETCH);
}

std::string CPU::get_instr_disassembly()
{
    return m_instr;
}