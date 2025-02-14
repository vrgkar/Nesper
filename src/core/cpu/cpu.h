#pragma once

#include "common/pch.h"
#include "common/component.h"

#include "bus/bus.h"

#include "cpustate.h"
#include "opcode.h"
#include "mode.h"
#include "interrupt.h"

class CPU final : public Component
{
public:

    /* Component Interface */
    bool read(uint8_t &byte, uint16_t addr) override;
    bool write(uint8_t byte, uint16_t addr) override;

    std::string_view get_id() const override { return "CPU"; }
    
    void broadcast(Event event) override;
    void service(Event event) override;

    /* CPU Functions */
    void step();
    void step_instruction();
    std::string get_instr_disassembly();

    std::string disassemble(uint16_t addr);

    void set_bus(Bus *bus) { m_bus = bus; }

private:
    void fetch();
    void decode();
    void execute();
    void interrupt();

private:

    CPUState m_state;
    Bus *m_bus = nullptr;
    std::string m_instr;
};
