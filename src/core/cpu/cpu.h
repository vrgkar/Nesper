#pragma once

#include <cstdlib>
#include <bitset>
#include <memory>
#include <string_view>

#include "../bus/bus.h"

#include "../system/component/component.h"
#include "cpuregister/cpuregister.h"
#include "state/state.h"
#include "opcode/opcode.h"
#include "mode/mode.h"
#include "interrupt/interrupt.h"

class CPU final : public Component
{
public:

    CPU();
    explicit CPU(Bus *bus);

    /* Component Interface */
    bool fetch(uint8_t &byte, uint16_t addr) override;
    bool commit(uint8_t byte, uint16_t addr) override;
    std::string_view get_id() override { return "CPU"; }

    void service(Event event) override;

    /* CPU Functions */
    void step();
    bool step_opcode() { return m_opcode->step(*this, m_r); }
    bool step_mode() { return m_mode->step(*this, m_r); }
    void poll_interrupts();

    void tick() { }
    void set_bus(Bus *bus) { m_bus = bus; }

    /* Communication interface through bus */
    uint8_t read(uint16_t addr) { return m_bus->read(*this, addr); }
    void write(uint8_t byte, uint16_t addr) { m_bus->write(*this, byte, addr); }

    bool is_active() const { return m_active; }

    void set_state(std::shared_ptr<State> state) { m_state = state; }
    void set_opcode(std::shared_ptr<Opcode> opcode) { m_opcode = opcode; }
    void set_mode(std::shared_ptr<Mode> mode) { m_mode = mode; }
    void execute_state() { m_state->execute(*this, m_r); }

    auto get_cycles() const { return m_cycles; }

    auto get_instruction_id() const { return m_opcode->get_id(); }
    auto get_mode_id() const { return m_mode->get_id(); }

    int m_cycles;
    int m_op;

    CPURegister m_r;


private:    
    std::shared_ptr<State> m_state;
    std::shared_ptr<Opcode> m_opcode;
    std::shared_ptr<Mode> m_mode;
    
    bool m_active;

    Bus *m_bus;
};
