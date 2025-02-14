#include "cpu.h"

void CPU::fetch()
{
    m_instr.clear();
    m_state.op = m_bus->cpu_read(m_state.r.pc++);
    m_state.process = CPUProcess::DECODE;
}

void CPU::execute()
{
    static int flag = 0;

    if (flag == 0)
    {
        if (!MODE_TABLE[std::to_underlying(m_state.mode)]->step(*m_bus, m_state))   
            ++flag;
        return;
    }

    if (flag == 1)
        if (!OPCODE_TABLE[std::to_underlying(m_state.opcode)]->step(*m_bus, m_state))
            ++flag;

    if (flag == 2)
    {
       // m_instr += std::format(" {:x} {:x} PC: {:x}", m_state.r.mar, m_state.r.mdr, m_state.r.pc);
        flag = 0;
        m_state.process = CPUProcess::FETCH;
    }
}

void CPU::interrupt()
{
    if (INTERRUPT_TABLE[std::to_underlying(m_state.interrupt)]->step(*m_bus, m_state))
        return;

    m_state.process = CPUProcess::FETCH;
}

void CPU::decode()
{
    switch (m_state.op) {
    case 0x0:
        m_state.opcode = CPUOpcode::BRK;
        m_state.mode = CPUMode::IMM;
        break;
    case 0x1:
        m_state.opcode = CPUOpcode::ORA;
        m_state.mode = CPUMode::IDX;
        break;
    case 0x2:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x3:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x4:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x5:
        m_state.opcode = CPUOpcode::ORA;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x6:
        m_state.opcode = CPUOpcode::ASL;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x7:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x8:
        m_state.opcode = CPUOpcode::PHP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x9:
        m_state.opcode = CPUOpcode::ORA;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xa:
        m_state.opcode = CPUOpcode::ASL;
        m_state.mode = CPUMode::ACC;
        break;
    case 0xb:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xc:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xd:
        m_state.opcode = CPUOpcode::ORA;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xe:
        m_state.opcode = CPUOpcode::ASL;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xf:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x10:
        m_state.opcode = CPUOpcode::BPL;
        m_state.mode = CPUMode::REL;
        break;
    case 0x11:
        m_state.opcode = CPUOpcode::ORA;
        m_state.mode = CPUMode::IDY;
        break;
    case 0x12:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x13:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x14:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x15:
        m_state.opcode = CPUOpcode::ORA;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x16:
        m_state.opcode = CPUOpcode::ASL;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x17:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x18:
        m_state.opcode = CPUOpcode::CLC;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x19:
        m_state.opcode = CPUOpcode::ORA;
        m_state.mode = CPUMode::ABY;
        break;
    case 0x1a:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x1b:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x1c:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x1d:
        m_state.opcode = CPUOpcode::ORA;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x1e:
        m_state.opcode = CPUOpcode::ASL;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x1f:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x20:
        m_state.opcode = CPUOpcode::JSR;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x21:
        m_state.opcode = CPUOpcode::AND;
        m_state.mode = CPUMode::IDX;
        break;
    case 0x22:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x23:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x24:
        m_state.opcode = CPUOpcode::BIT;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x25:
        m_state.opcode = CPUOpcode::AND;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x26:
        m_state.opcode = CPUOpcode::ROL;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x27:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x28:
        m_state.opcode = CPUOpcode::PLP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x29:
        m_state.opcode = CPUOpcode::AND;
        m_state.mode = CPUMode::IMM;
        break;
    case 0x2a:
        m_state.opcode = CPUOpcode::ROL;
        m_state.mode = CPUMode::ACC;
        break;
    case 0x2b:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x2c:
        m_state.opcode = CPUOpcode::BIT;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x2d:
        m_state.opcode = CPUOpcode::AND;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x2e:
        m_state.opcode = CPUOpcode::ROL;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x2f:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x30:
        m_state.opcode = CPUOpcode::BMI;
        m_state.mode = CPUMode::REL;
        break;
    case 0x31:
        m_state.opcode = CPUOpcode::AND;
        m_state.mode = CPUMode::IDY;
        break;
    case 0x32:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x33:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x34:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x35:
        m_state.opcode = CPUOpcode::AND;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x36:
        m_state.opcode = CPUOpcode::ROL;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x37:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x38:
        m_state.opcode = CPUOpcode::SEC;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x39:
        m_state.opcode = CPUOpcode::AND;
        m_state.mode = CPUMode::ABY;
        break;
    case 0x3a:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x3b:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x3c:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x3d:
        m_state.opcode = CPUOpcode::AND;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x3e:
        m_state.opcode = CPUOpcode::ROL;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x3f:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x40:
        m_state.opcode = CPUOpcode::RTI;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x41:
        m_state.opcode = CPUOpcode::EOR;
        m_state.mode = CPUMode::IDX;
        break;
    case 0x42:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x43:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x44:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x45:
        m_state.opcode = CPUOpcode::EOR;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x46:
        m_state.opcode = CPUOpcode::LSR;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x47:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x48:
        m_state.opcode = CPUOpcode::PHA;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x49:
        m_state.opcode = CPUOpcode::EOR;
        m_state.mode = CPUMode::IMM;
        break;
    case 0x4a:
        m_state.opcode = CPUOpcode::LSR;
        m_state.mode = CPUMode::ACC;
        break;
    case 0x4b:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x4c:
        m_state.opcode = CPUOpcode::JMP;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x4d:
        m_state.opcode = CPUOpcode::EOR;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x4e:
        m_state.opcode = CPUOpcode::LSR;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x4f:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x50:
        m_state.opcode = CPUOpcode::BVC;
        m_state.mode = CPUMode::REL;
        break;
    case 0x51:
        m_state.opcode = CPUOpcode::EOR;
        m_state.mode = CPUMode::IDY;
        break;
    case 0x52:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x53:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x54:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x55:
        m_state.opcode = CPUOpcode::EOR;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x56:
        m_state.opcode = CPUOpcode::LSR;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x57:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x58:
        m_state.opcode = CPUOpcode::CLI;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x59:
        m_state.opcode = CPUOpcode::EOR;
        m_state.mode = CPUMode::ABY;
        break;
    case 0x5a:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x5b:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x5c:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x5d:
        m_state.opcode = CPUOpcode::EOR;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x5e:
        m_state.opcode = CPUOpcode::LSR;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x5f:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x60:
        m_state.opcode = CPUOpcode::RTS;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x61:
        m_state.opcode = CPUOpcode::ADC;
        m_state.mode = CPUMode::IDX;
        break;
    case 0x62:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x63:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x64:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x65:
        m_state.opcode = CPUOpcode::ADC;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x66:
        m_state.opcode = CPUOpcode::ROR;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x67:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x68:
        m_state.opcode = CPUOpcode::PLA;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x69:
        m_state.opcode = CPUOpcode::ADC;
        m_state.mode = CPUMode::IMM;
        break;
    case 0x6a:
        m_state.opcode = CPUOpcode::ROR;
        m_state.mode = CPUMode::ACC;
        break;
    case 0x6b:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x6c:
        m_state.opcode = CPUOpcode::JMP;
        m_state.mode = CPUMode::IND;
        break;
    case 0x6d:
        m_state.opcode = CPUOpcode::ADC;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x6e:
        m_state.opcode = CPUOpcode::ROR;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x6f:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x70:
        m_state.opcode = CPUOpcode::BVS;
        m_state.mode = CPUMode::REL;
        break;
    case 0x71:
        m_state.opcode = CPUOpcode::ADC;
        m_state.mode = CPUMode::IDY;
        break;
    case 0x72:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x73:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x74:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x75:
        m_state.opcode = CPUOpcode::ADC;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x76:
        m_state.opcode = CPUOpcode::ROR;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x77:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x78:
        m_state.opcode = CPUOpcode::SEI;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x79:
        m_state.opcode = CPUOpcode::ADC;
        m_state.mode = CPUMode::ABY;
        break;
    case 0x7a:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x7b:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x7c:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x7d:
        m_state.opcode = CPUOpcode::ADC;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x7e:
        m_state.opcode = CPUOpcode::ROR;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x7f:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x80:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMM;
        break;
    case 0x81:
        m_state.opcode = CPUOpcode::STA;
        m_state.mode = CPUMode::IDX;
        break;
    case 0x82:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMM;
        break;
    case 0x83:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x84:
        m_state.opcode = CPUOpcode::STY;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x85:
        m_state.opcode = CPUOpcode::STA;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x86:
        m_state.opcode = CPUOpcode::STX;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0x87:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x88:
        m_state.opcode = CPUOpcode::DEY;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x89:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMM;
        break;
    case 0x8a:
        m_state.opcode = CPUOpcode::TXA;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x8b:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x8c:
        m_state.opcode = CPUOpcode::STY;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x8d:
        m_state.opcode = CPUOpcode::STA;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x8e:
        m_state.opcode = CPUOpcode::STX;
        m_state.mode = CPUMode::ABS;
        break;
    case 0x8f:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x90:
        m_state.opcode = CPUOpcode::BCC;
        m_state.mode = CPUMode::REL;
        break;
    case 0x91:
        m_state.opcode = CPUOpcode::STA;
        m_state.mode = CPUMode::IDY;
        break;
    case 0x92:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x93:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x94:
        m_state.opcode = CPUOpcode::STY;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x95:
        m_state.opcode = CPUOpcode::STA;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0x96:
        m_state.opcode = CPUOpcode::STX;
        m_state.mode = CPUMode::ZPY;
        break;
    case 0x97:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x98:
        m_state.opcode = CPUOpcode::TYA;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x99:
        m_state.opcode = CPUOpcode::STA;
        m_state.mode = CPUMode::ABY;
        break;
    case 0x9a:
        m_state.opcode = CPUOpcode::TXS;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x9b:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x9c:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x9d:
        m_state.opcode = CPUOpcode::STA;
        m_state.mode = CPUMode::ABX;
        break;
    case 0x9e:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0x9f:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xa0:
        m_state.opcode = CPUOpcode::LDY;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xa1:
        m_state.opcode = CPUOpcode::LDA;
        m_state.mode = CPUMode::IDX;
        break;
    case 0xa2:
        m_state.opcode = CPUOpcode::LDX;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xa3:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xa4:
        m_state.opcode = CPUOpcode::LDY;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0xa5:
        m_state.opcode = CPUOpcode::LDA;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0xa6:
        m_state.opcode = CPUOpcode::LDX;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0xa7:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xa8:
        m_state.opcode = CPUOpcode::TAY;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xa9:
        m_state.opcode = CPUOpcode::LDA;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xaa:
        m_state.opcode = CPUOpcode::TAX;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xab:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xac:
        m_state.opcode = CPUOpcode::LDY;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xad:
        m_state.opcode = CPUOpcode::LDA;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xae:
        m_state.opcode = CPUOpcode::LDX;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xaf:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xb0:
        m_state.opcode = CPUOpcode::BCS;
        m_state.mode = CPUMode::REL;
        break;
    case 0xb1:
        m_state.opcode = CPUOpcode::LDA;
        m_state.mode = CPUMode::IDY;
        break;
    case 0xb2:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xb3:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xb4:
        m_state.opcode = CPUOpcode::LDY;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0xb5:
        m_state.opcode = CPUOpcode::LDA;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0xb6:
        m_state.opcode = CPUOpcode::LDX;
        m_state.mode = CPUMode::ZPY;
        break;
    case 0xb7:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xb8:
        m_state.opcode = CPUOpcode::CLV;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xb9:
        m_state.opcode = CPUOpcode::LDA;
        m_state.mode = CPUMode::ABY;
        break;
    case 0xba:
        m_state.opcode = CPUOpcode::TSX;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xbb:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xbc:
        m_state.opcode = CPUOpcode::LDY;
        m_state.mode = CPUMode::ABX;
        break;
    case 0xbd:
        m_state.opcode = CPUOpcode::LDA;
        m_state.mode = CPUMode::ABX;
        break;
    case 0xbe:
        m_state.opcode = CPUOpcode::LDX;
        m_state.mode = CPUMode::ABY;
        break;
    case 0xbf:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xc0:
        m_state.opcode = CPUOpcode::CPY;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xc1:
        m_state.opcode = CPUOpcode::CMP;
        m_state.mode = CPUMode::IDX;
        break;
    case 0xc2:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xc3:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xc4:
        m_state.opcode = CPUOpcode::CPY;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0xc5:
        m_state.opcode = CPUOpcode::CMP;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0xc6:
        m_state.opcode = CPUOpcode::DEC;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0xc7:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xc8:
        m_state.opcode = CPUOpcode::INY;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xc9:
        m_state.opcode = CPUOpcode::CMP;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xca:
        m_state.opcode = CPUOpcode::DEX;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xcb:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xcc:
        m_state.opcode = CPUOpcode::CPY;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xcd:
        m_state.opcode = CPUOpcode::CMP;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xce:
        m_state.opcode = CPUOpcode::DEC;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xcf:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xd0:
        m_state.opcode = CPUOpcode::BNE;
        m_state.mode = CPUMode::REL;
        break;
    case 0xd1:
        m_state.opcode = CPUOpcode::CMP;
        m_state.mode = CPUMode::IDY;
        break;
    case 0xd2:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xd3:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xd4:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0xd5:
        m_state.opcode = CPUOpcode::CMP;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0xd6:
        m_state.opcode = CPUOpcode::DEC;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0xd7:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xd8:
        m_state.opcode = CPUOpcode::CLD;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xd9:
        m_state.opcode = CPUOpcode::CMP;
        m_state.mode = CPUMode::ABY;
        break;
    case 0xda:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xdb:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xdc:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ABX;
        break;
    case 0xdd:
        m_state.opcode = CPUOpcode::CMP;
        m_state.mode = CPUMode::ABX;
        break;
    case 0xde:
        m_state.opcode = CPUOpcode::DEC;
        m_state.mode = CPUMode::ABX;
        break;
    case 0xdf:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xe0:
        m_state.opcode = CPUOpcode::CPX;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xe1:
        m_state.opcode = CPUOpcode::SBC;
        m_state.mode = CPUMode::IDX;
        break;
    case 0xe2:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xe3:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xe4:
        m_state.opcode = CPUOpcode::CPX;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0xe5:
        m_state.opcode = CPUOpcode::SBC;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0xe6:
        m_state.opcode = CPUOpcode::INC;
        m_state.mode = CPUMode::ZPG;
        break;
    case 0xe7:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xe8:
        m_state.opcode = CPUOpcode::INX;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xe9:
        m_state.opcode = CPUOpcode::SBC;
        m_state.mode = CPUMode::IMM;
        break;
    case 0xea:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xeb:
        m_state.opcode = CPUOpcode::SBC;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xec:
        m_state.opcode = CPUOpcode::CPX;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xed:
        m_state.opcode = CPUOpcode::SBC;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xee:
        m_state.opcode = CPUOpcode::INC;
        m_state.mode = CPUMode::ABS;
        break;
    case 0xef:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xf0:
        m_state.opcode = CPUOpcode::BEQ;
        m_state.mode = CPUMode::REL;
        break;
    case 0xf1:
        m_state.opcode = CPUOpcode::SBC;
        m_state.mode = CPUMode::IDY;
        break;
    case 0xf2:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xf3:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xf4:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0xf5:
        m_state.opcode = CPUOpcode::SBC;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0xf6:
        m_state.opcode = CPUOpcode::INC;
        m_state.mode = CPUMode::ZPX;
        break;
    case 0xf7:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xf8:
        m_state.opcode = CPUOpcode::SED;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xf9:
        m_state.opcode = CPUOpcode::SBC;
        m_state.mode = CPUMode::ABY;
        break;
    case 0xfa:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xfb:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    case 0xfc:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::ABX;
        break;
    case 0xfd:
        m_state.opcode = CPUOpcode::SBC;
        m_state.mode = CPUMode::ABX;
        break;
    case 0xfe:
        m_state.opcode = CPUOpcode::INC;
        m_state.mode = CPUMode::ABX;
        break;
    case 0xff:
        m_state.opcode = CPUOpcode::NOP;
        m_state.mode = CPUMode::IMP;
        break;
    }

    m_instr += OPCODE_TABLE[std::to_underlying(m_state.opcode)]->get_id();
    m_state.process = CPUProcess::EXECUTE;
}