#include "cpu.h"

void CPU::fetch()
{
    m_opcode = m_bus->read_cpu(m_r.pc++);
    m_state = State::DECODE;
}

void CPU::decode()
{
    switch (m_opcode) 
    {
        case 0x00:         m_instr = Instruction::BRK;         m_mode = Mode::IMM;         break;
        case 0x01:         m_instr = Instruction::ORA;         m_mode = Mode::IDX;         break;
        case 0x02:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x03:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x04:         m_instr = Instruction::NOP;         m_mode = Mode::ZPG;         break;
        case 0x05:         m_instr = Instruction::ORA;         m_mode = Mode::ZPG;         break;
        case 0x06:         m_instr = Instruction::ASL;         m_mode = Mode::ZPG;         break;
        case 0x07:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x08:         m_instr = Instruction::PHP;         m_mode = Mode::IMP;         break;
        case 0x09:         m_instr = Instruction::ORA;         m_mode = Mode::IMM;         break;
        case 0x0a:         m_instr = Instruction::ASL;         m_mode = Mode::ACC;         break;
        case 0x0b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x0c:         m_instr = Instruction::NOP;         m_mode = Mode::ABS;         break;
        case 0x0d:         m_instr = Instruction::ORA;         m_mode = Mode::ABS;         break;
        case 0x0e:         m_instr = Instruction::ASL;         m_mode = Mode::ABS;         break;
        case 0x0f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x10:         m_instr = Instruction::BPL;         m_mode = Mode::REL;         break;
        case 0x11:         m_instr = Instruction::ORA;         m_mode = Mode::IDY;         break;
        case 0x12:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x13:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x14:         m_instr = Instruction::NOP;         m_mode = Mode::ZPX;         break;
        case 0x15:         m_instr = Instruction::ORA;         m_mode = Mode::ZPX;         break;
        case 0x16:         m_instr = Instruction::ASL;         m_mode = Mode::ZPX;         break;
        case 0x17:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x18:         m_instr = Instruction::CLC;         m_mode = Mode::IMP;         break;
        case 0x19:         m_instr = Instruction::ORA;         m_mode = Mode::ABY;         break;
        case 0x1a:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x1b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x1c:         m_instr = Instruction::NOP;         m_mode = Mode::ABX;         break;
        case 0x1d:         m_instr = Instruction::ORA;         m_mode = Mode::ABX;         break;
        case 0x1e:         m_instr = Instruction::ASL;         m_mode = Mode::ABX;         break;
        case 0x1f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x20:         m_instr = Instruction::JSR;         m_mode = Mode::ABS;         break;
        case 0x21:         m_instr = Instruction::AND;         m_mode = Mode::IDX;         break;
        case 0x22:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x23:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x24:         m_instr = Instruction::BIT;         m_mode = Mode::ZPG;         break;
        case 0x25:         m_instr = Instruction::AND;         m_mode = Mode::ZPG;         break;
        case 0x26:         m_instr = Instruction::ROL;         m_mode = Mode::ZPG;         break;
        case 0x27:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x28:         m_instr = Instruction::PLP;         m_mode = Mode::IMP;         break;
        case 0x29:         m_instr = Instruction::AND;         m_mode = Mode::IMM;         break;
        case 0x2a:         m_instr = Instruction::ROL;         m_mode = Mode::ACC;         break;
        case 0x2b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x2c:         m_instr = Instruction::BIT;         m_mode = Mode::ABS;         break;
        case 0x2d:         m_instr = Instruction::AND;         m_mode = Mode::ABS;         break;
        case 0x2e:         m_instr = Instruction::ROL;         m_mode = Mode::ABS;         break;
        case 0x2f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x30:         m_instr = Instruction::BMI;         m_mode = Mode::REL;         break;
        case 0x31:         m_instr = Instruction::AND;         m_mode = Mode::IDY;         break;
        case 0x32:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x33:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x34:         m_instr = Instruction::NOP;         m_mode = Mode::ZPX;         break;
        case 0x35:         m_instr = Instruction::AND;         m_mode = Mode::ZPX;         break;
        case 0x36:         m_instr = Instruction::ROL;         m_mode = Mode::ZPX;         break;
        case 0x37:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x38:         m_instr = Instruction::SEC;         m_mode = Mode::IMP;         break;
        case 0x39:         m_instr = Instruction::AND;         m_mode = Mode::ABY;         break;
        case 0x3a:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x3b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x3c:         m_instr = Instruction::NOP;         m_mode = Mode::ABX;         break;
        case 0x3d:         m_instr = Instruction::AND;         m_mode = Mode::ABX;         break;
        case 0x3e:         m_instr = Instruction::ROL;         m_mode = Mode::ABX;         break;
        case 0x3f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x40:         m_instr = Instruction::RTI;         m_mode = Mode::IMP;         break;
        case 0x41:         m_instr = Instruction::EOR;         m_mode = Mode::IDX;         break;
        case 0x42:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x43:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x44:         m_instr = Instruction::NOP;         m_mode = Mode::ZPG;         break;
        case 0x45:         m_instr = Instruction::EOR;         m_mode = Mode::ZPG;         break;
        case 0x46:         m_instr = Instruction::LSR;         m_mode = Mode::ZPG;         break;
        case 0x47:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x48:         m_instr = Instruction::PHA;         m_mode = Mode::IMP;         break;
        case 0x49:         m_instr = Instruction::EOR;         m_mode = Mode::IMM;         break;
        case 0x4a:         m_instr = Instruction::LSR;         m_mode = Mode::ACC;         break;
        case 0x4b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x4c:         m_instr = Instruction::JMP;         m_mode = Mode::ABS;         break;
        case 0x4d:         m_instr = Instruction::EOR;         m_mode = Mode::ABS;         break;
        case 0x4e:         m_instr = Instruction::LSR;         m_mode = Mode::ABS;         break;
        case 0x4f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x50:         m_instr = Instruction::BVC;         m_mode = Mode::REL;         break;
        case 0x51:         m_instr = Instruction::EOR;         m_mode = Mode::IDY;         break;
        case 0x52:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x53:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x54:         m_instr = Instruction::NOP;         m_mode = Mode::ZPX;         break;
        case 0x55:         m_instr = Instruction::EOR;         m_mode = Mode::ZPX;         break;
        case 0x56:         m_instr = Instruction::LSR;         m_mode = Mode::ZPX;         break;
        case 0x57:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x58:         m_instr = Instruction::CLI;         m_mode = Mode::IMP;         break;
        case 0x59:         m_instr = Instruction::EOR;         m_mode = Mode::ABY;         break;
        case 0x5a:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x5b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x5c:         m_instr = Instruction::NOP;         m_mode = Mode::ABX;         break;
        case 0x5d:         m_instr = Instruction::EOR;         m_mode = Mode::ABX;         break;
        case 0x5e:         m_instr = Instruction::LSR;         m_mode = Mode::ABX;         break;
        case 0x5f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x60:         m_instr = Instruction::RTS;         m_mode = Mode::IMP;         break;
        case 0x61:         m_instr = Instruction::ADC;         m_mode = Mode::IDX;         break;
        case 0x62:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x63:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x64:         m_instr = Instruction::NOP;         m_mode = Mode::ZPG;         break;
        case 0x65:         m_instr = Instruction::ADC;         m_mode = Mode::ZPG;         break;
        case 0x66:         m_instr = Instruction::ROR;         m_mode = Mode::ZPG;         break;
        case 0x67:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x68:         m_instr = Instruction::PLA;         m_mode = Mode::IMP;         break;
        case 0x69:         m_instr = Instruction::ADC;         m_mode = Mode::IMM;         break;
        case 0x6a:         m_instr = Instruction::ROR;         m_mode = Mode::ACC;         break;
        case 0x6b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x6c:         m_instr = Instruction::JMP;         m_mode = Mode::IND;         break;
        case 0x6d:         m_instr = Instruction::ADC;         m_mode = Mode::ABS;         break;
        case 0x6e:         m_instr = Instruction::ROR;         m_mode = Mode::ABS;         break;
        case 0x6f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x70:         m_instr = Instruction::BVS;         m_mode = Mode::REL;         break;
        case 0x71:         m_instr = Instruction::ADC;         m_mode = Mode::IDY;         break;
        case 0x72:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x73:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x74:         m_instr = Instruction::NOP;         m_mode = Mode::ZPX;         break;
        case 0x75:         m_instr = Instruction::ADC;         m_mode = Mode::ZPX;         break;
        case 0x76:         m_instr = Instruction::ROR;         m_mode = Mode::ZPX;         break;
        case 0x77:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x78:         m_instr = Instruction::SEI;         m_mode = Mode::IMP;         break;
        case 0x79:         m_instr = Instruction::ADC;         m_mode = Mode::ABY;         break;
        case 0x7a:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x7b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x7c:         m_instr = Instruction::NOP;         m_mode = Mode::ABX;         break;
        case 0x7d:         m_instr = Instruction::ADC;         m_mode = Mode::ABX;         break;
        case 0x7e:         m_instr = Instruction::ROR;         m_mode = Mode::ABX;         break;
        case 0x7f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x80:         m_instr = Instruction::NOP;         m_mode = Mode::IMM;         break;
        case 0x81:         m_instr = Instruction::STA;         m_mode = Mode::IDX;         break;
        case 0x82:         m_instr = Instruction::NOP;         m_mode = Mode::IMM;         break;
        case 0x83:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x84:         m_instr = Instruction::STY;         m_mode = Mode::ZPG;         break;
        case 0x85:         m_instr = Instruction::STA;         m_mode = Mode::ZPG;         break;
        case 0x86:         m_instr = Instruction::STX;         m_mode = Mode::ZPG;         break;
        case 0x87:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x88:         m_instr = Instruction::DEY;         m_mode = Mode::IMP;         break;
        case 0x89:         m_instr = Instruction::NOP;         m_mode = Mode::IMM;         break;
        case 0x8a:         m_instr = Instruction::TXA;         m_mode = Mode::IMP;         break;
        case 0x8b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x8c:         m_instr = Instruction::STY;         m_mode = Mode::ABS;         break;
        case 0x8d:         m_instr = Instruction::STA;         m_mode = Mode::ABS;         break;
        case 0x8e:         m_instr = Instruction::STX;         m_mode = Mode::ABS;         break;
        case 0x8f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x90:         m_instr = Instruction::BCC;         m_mode = Mode::REL;         break;
        case 0x91:         m_instr = Instruction::STA;         m_mode = Mode::IDY;         break;
        case 0x92:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x93:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x94:         m_instr = Instruction::STY;         m_mode = Mode::ZPX;         break;
        case 0x95:         m_instr = Instruction::STA;         m_mode = Mode::ZPX;         break;
        case 0x96:         m_instr = Instruction::STX;         m_mode = Mode::ZPY;         break;
        case 0x97:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x98:         m_instr = Instruction::TYA;         m_mode = Mode::IMP;         break;
        case 0x99:         m_instr = Instruction::STA;         m_mode = Mode::ABY;         break;
        case 0x9a:         m_instr = Instruction::TXS;         m_mode = Mode::IMP;         break;
        case 0x9b:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x9c:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x9d:         m_instr = Instruction::STA;         m_mode = Mode::ABX;         break;
        case 0x9e:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0x9f:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xa0:         m_instr = Instruction::LDY;         m_mode = Mode::IMM;         break;
        case 0xa1:         m_instr = Instruction::LDA;         m_mode = Mode::IDX;         break;
        case 0xa2:         m_instr = Instruction::LDX;         m_mode = Mode::IMM;         break;
        case 0xa3:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xa4:         m_instr = Instruction::LDY;         m_mode = Mode::ZPG;         break;
        case 0xa5:         m_instr = Instruction::LDA;         m_mode = Mode::ZPG;         break;
        case 0xa6:         m_instr = Instruction::LDX;         m_mode = Mode::ZPG;         break;
        case 0xa7:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xa8:         m_instr = Instruction::TAY;         m_mode = Mode::IMP;         break;
        case 0xa9:         m_instr = Instruction::LDA;         m_mode = Mode::IMM;         break;
        case 0xaa:         m_instr = Instruction::TAX;         m_mode = Mode::IMP;         break;
        case 0xab:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xac:         m_instr = Instruction::LDY;         m_mode = Mode::ABS;         break;
        case 0xad:         m_instr = Instruction::LDA;         m_mode = Mode::ABS;         break;
        case 0xae:         m_instr = Instruction::LDX;         m_mode = Mode::ABS;         break;
        case 0xaf:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xb0:         m_instr = Instruction::BCS;         m_mode = Mode::REL;         break;
        case 0xb1:         m_instr = Instruction::LDA;         m_mode = Mode::IDY;         break;
        case 0xb2:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xb3:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xb4:         m_instr = Instruction::LDY;         m_mode = Mode::ZPX;         break;
        case 0xb5:         m_instr = Instruction::LDA;         m_mode = Mode::ZPX;         break;
        case 0xb6:         m_instr = Instruction::LDX;         m_mode = Mode::ZPY;         break;
        case 0xb7:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xb8:         m_instr = Instruction::CLV;         m_mode = Mode::IMP;         break;
        case 0xb9:         m_instr = Instruction::LDA;         m_mode = Mode::ABY;         break;
        case 0xba:         m_instr = Instruction::TSX;         m_mode = Mode::IMP;         break;
        case 0xbb:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xbc:         m_instr = Instruction::LDY;         m_mode = Mode::ABX;         break;
        case 0xbd:         m_instr = Instruction::LDA;         m_mode = Mode::ABX;         break;
        case 0xbe:         m_instr = Instruction::LDX;         m_mode = Mode::ABY;         break;
        case 0xbf:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xc0:         m_instr = Instruction::CPY;         m_mode = Mode::IMM;         break;
        case 0xc1:         m_instr = Instruction::CMP;         m_mode = Mode::IDX;         break;
        case 0xc2:         m_instr = Instruction::NOP;         m_mode = Mode::IMM;         break;
        case 0xc3:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xc4:         m_instr = Instruction::CPY;         m_mode = Mode::ZPG;         break;
        case 0xc5:         m_instr = Instruction::CMP;         m_mode = Mode::ZPG;         break;
        case 0xc6:         m_instr = Instruction::DEC;         m_mode = Mode::ZPG;         break;
        case 0xc7:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xc8:         m_instr = Instruction::INY;         m_mode = Mode::IMP;         break;
        case 0xc9:         m_instr = Instruction::CMP;         m_mode = Mode::IMM;         break;
        case 0xca:         m_instr = Instruction::DEX;         m_mode = Mode::IMP;         break;
        case 0xcb:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xcc:         m_instr = Instruction::CPY;         m_mode = Mode::ABS;         break;
        case 0xcd:         m_instr = Instruction::CMP;         m_mode = Mode::ABS;         break;
        case 0xce:         m_instr = Instruction::DEC;         m_mode = Mode::ABS;         break;
        case 0xcf:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xd0:         m_instr = Instruction::BNE;         m_mode = Mode::REL;         break;
        case 0xd1:         m_instr = Instruction::CMP;         m_mode = Mode::IDY;         break;
        case 0xd2:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xd3:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xd4:         m_instr = Instruction::NOP;         m_mode = Mode::ZPX;         break;
        case 0xd5:         m_instr = Instruction::CMP;         m_mode = Mode::ZPX;         break;
        case 0xd6:         m_instr = Instruction::DEC;         m_mode = Mode::ZPX;         break;
        case 0xd7:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xd8:         m_instr = Instruction::CLD;         m_mode = Mode::IMP;         break;
        case 0xd9:         m_instr = Instruction::CMP;         m_mode = Mode::ABY;         break;
        case 0xda:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xdb:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xdc:         m_instr = Instruction::NOP;         m_mode = Mode::ABX;         break;
        case 0xdd:         m_instr = Instruction::CMP;         m_mode = Mode::ABX;         break;
        case 0xde:         m_instr = Instruction::DEC;         m_mode = Mode::ABX;         break;
        case 0xdf:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xe0:         m_instr = Instruction::CPX;         m_mode = Mode::IMM;         break;
        case 0xe1:         m_instr = Instruction::SBC;         m_mode = Mode::IDX;         break;
        case 0xe2:         m_instr = Instruction::NOP;         m_mode = Mode::IMM;         break;
        case 0xe3:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xe4:         m_instr = Instruction::CPX;         m_mode = Mode::ZPG;         break;
        case 0xe5:         m_instr = Instruction::SBC;         m_mode = Mode::ZPG;         break;
        case 0xe6:         m_instr = Instruction::INC;         m_mode = Mode::ZPG;         break;
        case 0xe7:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xe8:         m_instr = Instruction::INX;         m_mode = Mode::IMP;         break;
        case 0xe9:         m_instr = Instruction::SBC;         m_mode = Mode::IMM;         break;
        case 0xea:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xeb:         m_instr = Instruction::SBC;         m_mode = Mode::IMP;         break;
        case 0xec:         m_instr = Instruction::CPX;         m_mode = Mode::ABS;         break;
        case 0xed:         m_instr = Instruction::SBC;         m_mode = Mode::ABS;         break;
        case 0xee:         m_instr = Instruction::INC;         m_mode = Mode::ABS;         break;
        case 0xef:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xf0:         m_instr = Instruction::BEQ;         m_mode = Mode::REL;         break;
        case 0xf1:         m_instr = Instruction::SBC;         m_mode = Mode::IDY;         break;
        case 0xf2:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xf3:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xf4:         m_instr = Instruction::NOP;         m_mode = Mode::ZPX;         break;
        case 0xf5:         m_instr = Instruction::SBC;         m_mode = Mode::ZPX;         break;
        case 0xf6:         m_instr = Instruction::INC;         m_mode = Mode::ZPX;         break;
        case 0xf7:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xf8:         m_instr = Instruction::SED;         m_mode = Mode::IMP;         break;
        case 0xf9:         m_instr = Instruction::SBC;         m_mode = Mode::ABY;         break;
        case 0xfa:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xfb:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
        case 0xfc:         m_instr = Instruction::NOP;         m_mode = Mode::ABX;         break;
        case 0xfd:         m_instr = Instruction::SBC;         m_mode = Mode::ABX;         break;
        case 0xfe:         m_instr = Instruction::INC;         m_mode = Mode::ABX;         break;
        case 0xff:         m_instr = Instruction::NOP;         m_mode = Mode::IMP;         break;
    }

    m_state = State::EXECUTE_MODE;
}

/* Execute Mode */

void CPU::execute_mode()
{
    bool done = false;

    switch (m_mode) 
    {
    case Mode::IMP:         done = IMP();       break;
    case Mode::IMM:         done = IMM();       break;
    case Mode::ZPG:         done = ZPG();       break;
    case Mode::ZPX:         done = ZPX();       break;
    case Mode::ZPY:         done = ZPY();       break;
    case Mode::ACC:         done = ACC();       break;
    case Mode::REL:         done = REL();       break;
    case Mode::ABS:         done = ABS();       break;
    case Mode::ABX:         done = ABX();       break;
    case Mode::ABY:         done = ABY();       break;
    case Mode::IND:         done = IND();       break;
    case Mode::IDX:         done = IDX();       break;
    case Mode::IDY:         done = IDY();       break;  
    }

    if (done)
        m_state = State::EXECUTE_INSTRUCTION;    
}

/* Execute Instruction */

void CPU::execute_instruction()
{
    bool done = false;

    switch (m_instr) 
    {
    case Instruction::ADC:         done = ADC();        break;
    case Instruction::AND:         done = AND();        break;
    case Instruction::ASL:         done = ASL();        break;
    case Instruction::BCC:         done = BCC();        break;
    case Instruction::BCS:         done = BCS();        break;
    case Instruction::BEQ:         done = BEQ();        break;
    case Instruction::BIT:         done = BIT();        break;
    case Instruction::BMI:         done = BMI();        break;
    case Instruction::BNE:         done = BNE();        break;
    case Instruction::BPL:         done = BPL();        break;
    case Instruction::BRK:         done = BRK();        break;
    case Instruction::BVC:         done = BVC();        break;
    case Instruction::BVS:         done = BVS();        break;
    case Instruction::CLC:         done = CLC();        break;
    case Instruction::CLD:         done = CLD();        break;
    case Instruction::CLI:         done = CLI();        break;
    case Instruction::CLV:         done = CLV();        break;
    case Instruction::CMP:         done = CMP();        break;
    case Instruction::CPX:         done = CPX();        break;
    case Instruction::CPY:         done = CPY();        break;
    case Instruction::DEC:         done = DEC();        break;
    case Instruction::DEX:         done = DEX();        break;
    case Instruction::DEY:         done = DEY();        break;
    case Instruction::EOR:         done = EOR();        break;
    case Instruction::INC:         done = INC();        break;
    case Instruction::INX:         done = INX();        break;
    case Instruction::INY:         done = INY();        break;
    case Instruction::JMP:         done = JMP();        break;
    case Instruction::JSR:         done = JSR();        break;
    case Instruction::LDA:         done = LDA();        break;
    case Instruction::LDX:         done = LDX();        break;
    case Instruction::LDY:         done = LDY();        break;
    case Instruction::LSR:         done = LSR();        break;
    case Instruction::NOP:         done = NOP();        break;
    case Instruction::ORA:         done = ORA();        break;
    case Instruction::PHA:         done = PHA();        break;
    case Instruction::PHP:         done = PHP();        break;
    case Instruction::PLA:         done = PLA();        break;
    case Instruction::PLP:         done = PLP();        break;
    case Instruction::ROL:         done = ROL();        break;
    case Instruction::ROR:         done = ROR();        break;
    case Instruction::RTI:         done = RTI();        break;
    case Instruction::RTS:         done = RTS();        break;
    case Instruction::SBC:         done = SBC();        break;
    case Instruction::SEC:         done = SEC();        break;
    case Instruction::SED:         done = SED();        break;
    case Instruction::SEI:         done = SEI();        break;
    case Instruction::STA:         done = STA();        break;
    case Instruction::STX:         done = STX();        break;
    case Instruction::STY:         done = STY();        break;
    case Instruction::TAX:         done = TAX();        break;
    case Instruction::TAY:         done = TAY();        break;
    case Instruction::TSX:         done = TSX();        break;
    case Instruction::TXA:         done = TXA();        break;
    case Instruction::TXS:         done = TXS();        break;
    case Instruction::TYA:         done = TYA();        break;      
    }

    if (done)
        m_state = State::FETCH;
}

/* Execute Interrupt */

void CPU::interrupt()
{
    bool done = false;

    switch (m_interrupt)
    {
    case Interrupt::RST:        done = RST();       break;
    case Interrupt::IRQ:        done = IRQ();       break;
    case Interrupt::NMI:        done = NMI();       break;
    }

    if (done)
        m_state = State::FETCH;
}