#pragma once

#include "common/pch.h"
#include "common/component.h"

#include "bus/bus.h"

#include "cpustate.h"

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

private:

    /* Instructions */
    bool ADC();
    bool AND();
    bool ASL();
    bool BCC();
    bool BCS();
    bool BEQ();
    bool BIT();
    bool BMI();
    bool BNE();
    bool BPL();
    bool BRK();
    bool BVC();
    bool BVS();
    bool CLC();
    bool CLD();
    bool CLI();
    bool CLV();
    bool CMP();
    bool CPX();
    bool CPY();
    bool DEC();
    bool DEX();
    bool DEY();
    bool EOR();
    bool INC();
    bool INX();
    bool INY();
    bool JMP();
    bool JSR();
    bool LDA();
    bool LDX();
    bool LDY();
    bool LSR();
    bool NOP();
    bool ORA();
    bool PHA();
    bool PHP();
    bool PLA();
    bool PLP();
    bool ROL();
    bool ROR();
    bool RTI();
    bool RTS();
    bool SBC();
    bool SEC();
    bool SED();
    bool SEI();
    bool STA();
    bool STX();
    bool STY();
    bool TAX();
    bool TAY();
    bool TSX();
    bool TXA();
    bool TXS();
    bool TYA();

    /* Modes */
    bool IMP();
    bool IMM();
    bool ZPG();
    bool ZPX();
    bool ZPY();
    bool ACC();
    bool REL();
    bool ABS();
    bool ABX();
    bool ABY();
    bool IDX();
    bool IDY();

    /* Interrupts */
    bool RST();
    bool IRQ();
    bool NMI();

};
