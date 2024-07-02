#pragma once

#include "cpuregister.h"

enum class CPUMode
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
    IDY,
};

enum class CPUOpcode
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
    TYA,
};

enum class CPUProcess
{
    FETCH,
    DECODE,
    EXECUTE,
    INTERRUPT
};

enum class CPUInterrupt
{
    RST,
    BRK,
    IRQ,
    NMI
};

struct CPUState
{
    CPUOpcode opcode = CPUOpcode::NOP;
    CPUMode mode = CPUMode::IMP;
    
    CPUProcess process = CPUProcess::INTERRUPT;
    CPUInterrupt interrupt = CPUInterrupt::RST;

    CPURegister r;

    uint8_t op = 0x00;
};