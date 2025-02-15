#pragma once

#include "common/pch.h"
#include "common/defs.h"
#include "common/component.h"

#include "bus/bus.h"

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
    struct Register
    {
        enum Status : unsigned
        {
            C,
            Z,
            I,
            D,
            B,
            X,
            V,
            N
        };
        
        void set(Status f, bool b)  { sr = (sr & ~(1u << f)) | (b << f); }
        bool test(Status f)         { return sr & (1u << f); }
        void reset()                { sp -= 3u; set(I, true); }

        /* Accumulator Register */
        uint8_t a = 0x0u;
    
        /* Index Registers */
        uint8_t x = 0x0u;
        uint8_t y = 0x0u;
    
        /* Program Counter */
        uint16_t pc = 0xC000u;
    
        /* Stack Pointer */
        uint8_t sp = 0xFDu;
    
        /* Status Register */
        uint8_t sr = 0x24u;
    
        /* Memory Address Register */
        uint16_t mar;
    
        /* Memory Data Register */
        uint8_t mdr;        
        
    } m_r;
    
    enum class State
    {
        FETCH,
        DECODE,
        EXECUTE_MODE,
        EXECUTE_INSTRUCTION,
        INTERRUPT
    } m_state = State::FETCH;
    
    enum class Mode
    {
        IMP,
        IMM,
        ZPG,
        ZPX,
        ZPY,
        ACC,
        REL,
        ABS,
        ABX,
        ABY,
        IND,
        IDX,
        IDY
    } m_mode = Mode::IMP;

    enum class Instruction
    {
        ADC,
        AND,
        ASL,
        BCC,
        BCS,
        BEQ,
        BIT,
        BMI,
        BNE,
        BPL,
        BRK,
        BVC,
        BVS,
        CLC,
        CLD,
        CLI,
        CLV,
        CMP,
        CPX,
        CPY,
        DEC,
        DEX,
        DEY,
        EOR,
        INC,
        INX,
        INY,
        JMP,
        JSR,
        LDA,
        LDX,
        LDY,
        LSR,
        NOP,
        ORA,
        PHA,
        PHP,
        PLA,
        PLP,
        ROL,
        ROR,
        RTI,
        RTS,
        SBC,
        SEC,
        SED,
        SEI,
        STA,
        STX,
        STY,
        TAX,
        TAY,
        TSX,
        TXA,
        TXS,
        TYA
    } m_instr = Instruction::NOP;

    enum class Interrupt
    {
        RST,
        IRQ,
        NMI
    } m_interrupt = Interrupt::RST;

    /* Opcode of current instruction */
    uint8_t m_opcode;

    /* Cycles Elapsed */
    int m_cycles = 0;

    /* Bus Reference */
    Bus *m_bus = nullptr;

private:

    /* States */
    void fetch();
    void decode();
    void execute_mode();
    void execute_instruction();
    void interrupt();

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
    bool IND();
    bool IDX();
    bool IDY();

    /* Interrupts */
    bool RST();
    bool IRQ();
    bool NMI();
};
