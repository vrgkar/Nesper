#pragma once

#include "common/pch.h"
#include "common/defs.h"
#include "common/component.h"

#include "bus/bus.h"

#include <fstream>

class CPU final : public Component
{
public:
    /* Component Interface */
    uint8_t read(uint16_t addr) override;
    void write(uint8_t byte, uint16_t addr) override;

    std::string_view get_id() const override { return "CPU"; }

    void process_events() override;
    
    /* CPU Functions */
    void step();
    void step_instruction();
    std::string get_instr_disassembly();

    std::string disassemble(uint16_t addr);

    void connect(Bus *bus) { m_bus = bus; m_bus->connect(this); }

    void suspend()                  { push_state(m_state); m_state = State::SUSPEND; }
    void resume()                   { m_state = top_state(); pop_state(); }

private:

    std::ofstream m_log;
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
        uint16_t pc;
    
        /* Stack Pointer */
        uint8_t sp = 0;
    
        /* Status Register */
        uint8_t sr = 0x24u;
    
        /* Memory Address Register */
        uint16_t mar;
    
        /* Memory Data Register */
        uint8_t mdr;
        
        /* OAM DMA Register */
        uint8_t oamdma = 0;

        /* Joystick Strobe Register */
        uint8_t joystrobe = 0;

    } m_r;
    
    enum class State
    {
        FETCH,
        DECODE,
        EXECUTE_MODE,
        EXECUTE_INSTRUCTION,
        INTERRUPT,
        SUSPEND
    } m_state = State::INTERRUPT;

    std::vector<State> m_state_stack = {};

    void push_state(State state)    { m_state_stack.push_back(state); }
    State top_state()               { return m_state_stack.back(); }
    void pop_state()                { m_state_stack.pop_back(); }
    
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
    } m_mode;

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
    } m_instr;

    enum class Interrupt
    {
        RST,
        IRQ,
        NMI,
    } m_interrupt = Interrupt::RST;

    /* Opcode of current instruction */
    uint8_t m_opcode;

    /* Cycles Elapsed */
    int m_cycles = 0;

    /* Bus Reference */
    Bus *m_bus = nullptr;

private:
    /* Utility Functions */
    std::string instruction_to_string(Instruction instr);
    std::string mode_to_string(Mode mode);

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
