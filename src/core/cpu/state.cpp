#include "state.h"
#include "../cpu.h"

#include <iostream>
#include <iomanip>

void IdleState::execute(CPU &cpu, CPURegister &r) const
{
    return;
}

void ReadyState::execute(CPU &cpu, CPURegister &r) const
{
    cpu.set_state(std::make_shared<FetchState>());
    cpu.execute_state();
}

void FetchState::execute(CPU &cpu, CPURegister &r) const
{
    /* Fetch opcode from memory */
    cpu.m_op = cpu.read(r.pc++);

    cpu.set_state(std::make_shared<DecodeState>());
    cpu.execute_state();
}

void DecodeState::execute(CPU &cpu, CPURegister &r) const
{

    switch (cpu.m_op) {
    case 0x0:
        cpu.set_opcode(std::make_shared<BRK>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0x1:
        cpu.set_opcode(std::make_shared<ORA>());
        cpu.set_mode(std::make_shared<IDX>());
        break;
    case 0x2:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x3:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x4:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x5:
        cpu.set_opcode(std::make_shared<ORA>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x6:
        cpu.set_opcode(std::make_shared<ASL>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x7:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x8:
        cpu.set_opcode(std::make_shared<PHP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x9:
        cpu.set_opcode(std::make_shared<ORA>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xa:
        cpu.set_opcode(std::make_shared<ASL>());
        cpu.set_mode(std::make_shared<ACC>());
        break;
    case 0xb:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xc:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xd:
        cpu.set_opcode(std::make_shared<ORA>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xe:
        cpu.set_opcode(std::make_shared<ASL>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xf:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x10:
        cpu.set_opcode(std::make_shared<BPL>());
        cpu.set_mode(std::make_shared<REL>());
        break;
    case 0x11:
        cpu.set_opcode(std::make_shared<ORA>());
        cpu.set_mode(std::make_shared<IDY>());
        break;
    case 0x12:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x13:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x14:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x15:
        cpu.set_opcode(std::make_shared<ORA>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x16:
        cpu.set_opcode(std::make_shared<ASL>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x17:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x18:
        cpu.set_opcode(std::make_shared<CLC>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x19:
        cpu.set_opcode(std::make_shared<ORA>());
        cpu.set_mode(std::make_shared<ABY>());
        break;
    case 0x1a:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x1b:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x1c:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x1d:
        cpu.set_opcode(std::make_shared<ORA>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x1e:
        cpu.set_opcode(std::make_shared<ASL>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x1f:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x20:
        cpu.set_opcode(std::make_shared<JSR>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x21:
        cpu.set_opcode(std::make_shared<AND>());
        cpu.set_mode(std::make_shared<IDX>());
        break;
    case 0x22:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x23:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x24:
        cpu.set_opcode(std::make_shared<BIT>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x25:
        cpu.set_opcode(std::make_shared<AND>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x26:
        cpu.set_opcode(std::make_shared<ROL>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x27:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x28:
        cpu.set_opcode(std::make_shared<PLP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x29:
        cpu.set_opcode(std::make_shared<AND>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0x2a:
        cpu.set_opcode(std::make_shared<ROL>());
        cpu.set_mode(std::make_shared<ACC>());
        break;
    case 0x2b:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x2c:
        cpu.set_opcode(std::make_shared<BIT>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x2d:
        cpu.set_opcode(std::make_shared<AND>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x2e:
        cpu.set_opcode(std::make_shared<ROL>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x2f:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x30:
        cpu.set_opcode(std::make_shared<BMI>());
        cpu.set_mode(std::make_shared<REL>());
        break;
    case 0x31:
        cpu.set_opcode(std::make_shared<AND>());
        cpu.set_mode(std::make_shared<IDY>());
        break;
    case 0x32:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x33:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x34:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x35:
        cpu.set_opcode(std::make_shared<AND>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x36:
        cpu.set_opcode(std::make_shared<ROL>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x37:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x38:
        cpu.set_opcode(std::make_shared<SEC>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x39:
        cpu.set_opcode(std::make_shared<AND>());
        cpu.set_mode(std::make_shared<ABY>());
        break;
    case 0x3a:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x3b:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x3c:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x3d:
        cpu.set_opcode(std::make_shared<AND>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x3e:
        cpu.set_opcode(std::make_shared<ROL>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x3f:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x40:
        cpu.set_opcode(std::make_shared<RTI>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x41:
        cpu.set_opcode(std::make_shared<EOR>());
        cpu.set_mode(std::make_shared<IDX>());
        break;
    case 0x42:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x43:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x44:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x45:
        cpu.set_opcode(std::make_shared<EOR>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x46:
        cpu.set_opcode(std::make_shared<LSR>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x47:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x48:
        cpu.set_opcode(std::make_shared<PHA>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x49:
        cpu.set_opcode(std::make_shared<EOR>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0x4a:
        cpu.set_opcode(std::make_shared<LSR>());
        cpu.set_mode(std::make_shared<ACC>());
        break;
    case 0x4b:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x4c:
        cpu.set_opcode(std::make_shared<JMP>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x4d:
        cpu.set_opcode(std::make_shared<EOR>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x4e:
        cpu.set_opcode(std::make_shared<LSR>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x4f:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x50:
        cpu.set_opcode(std::make_shared<BVC>());
        cpu.set_mode(std::make_shared<REL>());
        break;
    case 0x51:
        cpu.set_opcode(std::make_shared<EOR>());
        cpu.set_mode(std::make_shared<IDY>());
        break;
    case 0x52:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x53:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x54:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x55:
        cpu.set_opcode(std::make_shared<EOR>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x56:
        cpu.set_opcode(std::make_shared<LSR>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x57:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x58:
        cpu.set_opcode(std::make_shared<CLI>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x59:
        cpu.set_opcode(std::make_shared<EOR>());
        cpu.set_mode(std::make_shared<ABY>());
        break;
    case 0x5a:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x5b:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x5c:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x5d:
        cpu.set_opcode(std::make_shared<EOR>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x5e:
        cpu.set_opcode(std::make_shared<LSR>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x5f:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x60:
        cpu.set_opcode(std::make_shared<RTS>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x61:
        cpu.set_opcode(std::make_shared<ADC>());
        cpu.set_mode(std::make_shared<IDX>());
        break;
    case 0x62:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x63:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x64:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x65:
        cpu.set_opcode(std::make_shared<ADC>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x66:
        cpu.set_opcode(std::make_shared<ROR>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x67:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x68:
        cpu.set_opcode(std::make_shared<PLA>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x69:
        cpu.set_opcode(std::make_shared<ADC>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0x6a:
        cpu.set_opcode(std::make_shared<ROR>());
        cpu.set_mode(std::make_shared<ACC>());
        break;
    case 0x6b:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x6c:
        cpu.set_opcode(std::make_shared<JMP>());
        cpu.set_mode(std::make_shared<IND>());
        break;
    case 0x6d:
        cpu.set_opcode(std::make_shared<ADC>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x6e:
        cpu.set_opcode(std::make_shared<ROR>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x6f:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x70:
        cpu.set_opcode(std::make_shared<BVS>());
        cpu.set_mode(std::make_shared<REL>());
        break;
    case 0x71:
        cpu.set_opcode(std::make_shared<ADC>());
        cpu.set_mode(std::make_shared<IDY>());
        break;
    case 0x72:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x73:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x74:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x75:
        cpu.set_opcode(std::make_shared<ADC>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x76:
        cpu.set_opcode(std::make_shared<ROR>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x77:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x78:
        cpu.set_opcode(std::make_shared<SEI>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x79:
        cpu.set_opcode(std::make_shared<ADC>());
        cpu.set_mode(std::make_shared<ABY>());
        break;
    case 0x7a:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x7b:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x7c:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x7d:
        cpu.set_opcode(std::make_shared<ADC>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x7e:
        cpu.set_opcode(std::make_shared<ROR>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x7f:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x80:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0x81:
        cpu.set_opcode(std::make_shared<STA>());
        cpu.set_mode(std::make_shared<IDX>());
        break;
    case 0x82:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0x83:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x84:
        cpu.set_opcode(std::make_shared<STY>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x85:
        cpu.set_opcode(std::make_shared<STA>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x86:
        cpu.set_opcode(std::make_shared<STX>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0x87:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x88:
        cpu.set_opcode(std::make_shared<DEY>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x89:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0x8a:
        cpu.set_opcode(std::make_shared<TXA>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x8b:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x8c:
        cpu.set_opcode(std::make_shared<STY>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x8d:
        cpu.set_opcode(std::make_shared<STA>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x8e:
        cpu.set_opcode(std::make_shared<STX>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0x8f:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x90:
        cpu.set_opcode(std::make_shared<BCC>());
        cpu.set_mode(std::make_shared<REL>());
        break;
    case 0x91:
        cpu.set_opcode(std::make_shared<STA>());
        cpu.set_mode(std::make_shared<IDY>());
        break;
    case 0x92:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x93:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x94:
        cpu.set_opcode(std::make_shared<STY>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x95:
        cpu.set_opcode(std::make_shared<STA>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0x96:
        cpu.set_opcode(std::make_shared<STX>());
        cpu.set_mode(std::make_shared<ZPY>());
        break;
    case 0x97:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x98:
        cpu.set_opcode(std::make_shared<TYA>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x99:
        cpu.set_opcode(std::make_shared<STA>());
        cpu.set_mode(std::make_shared<ABY>());
        break;
    case 0x9a:
        cpu.set_opcode(std::make_shared<TXS>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x9b:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x9c:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x9d:
        cpu.set_opcode(std::make_shared<STA>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0x9e:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0x9f:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xa0:
        cpu.set_opcode(std::make_shared<LDY>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xa1:
        cpu.set_opcode(std::make_shared<LDA>());
        cpu.set_mode(std::make_shared<IDX>());
        break;
    case 0xa2:
        cpu.set_opcode(std::make_shared<LDX>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xa3:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xa4:
        cpu.set_opcode(std::make_shared<LDY>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0xa5:
        cpu.set_opcode(std::make_shared<LDA>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0xa6:
        cpu.set_opcode(std::make_shared<LDX>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0xa7:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xa8:
        cpu.set_opcode(std::make_shared<TAY>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xa9:
        cpu.set_opcode(std::make_shared<LDA>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xaa:
        cpu.set_opcode(std::make_shared<TAX>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xab:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xac:
        cpu.set_opcode(std::make_shared<LDY>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xad:
        cpu.set_opcode(std::make_shared<LDA>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xae:
        cpu.set_opcode(std::make_shared<LDX>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xaf:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xb0:
        cpu.set_opcode(std::make_shared<BCS>());
        cpu.set_mode(std::make_shared<REL>());
        break;
    case 0xb1:
        cpu.set_opcode(std::make_shared<LDA>());
        cpu.set_mode(std::make_shared<IDY>());
        break;
    case 0xb2:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xb3:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xb4:
        cpu.set_opcode(std::make_shared<LDY>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0xb5:
        cpu.set_opcode(std::make_shared<LDA>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0xb6:
        cpu.set_opcode(std::make_shared<LDX>());
        cpu.set_mode(std::make_shared<ZPY>());
        break;
    case 0xb7:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xb8:
        cpu.set_opcode(std::make_shared<CLV>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xb9:
        cpu.set_opcode(std::make_shared<LDA>());
        cpu.set_mode(std::make_shared<ABY>());
        break;
    case 0xba:
        cpu.set_opcode(std::make_shared<TSX>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xbb:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xbc:
        cpu.set_opcode(std::make_shared<LDY>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0xbd:
        cpu.set_opcode(std::make_shared<LDA>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0xbe:
        cpu.set_opcode(std::make_shared<LDX>());
        cpu.set_mode(std::make_shared<ABY>());
        break;
    case 0xbf:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xc0:
        cpu.set_opcode(std::make_shared<CPY>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xc1:
        cpu.set_opcode(std::make_shared<CMP>());
        cpu.set_mode(std::make_shared<IDX>());
        break;
    case 0xc2:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xc3:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xc4:
        cpu.set_opcode(std::make_shared<CPY>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0xc5:
        cpu.set_opcode(std::make_shared<CMP>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0xc6:
        cpu.set_opcode(std::make_shared<DEC>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0xc7:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xc8:
        cpu.set_opcode(std::make_shared<INY>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xc9:
        cpu.set_opcode(std::make_shared<CMP>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xca:
        cpu.set_opcode(std::make_shared<DEX>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xcb:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xcc:
        cpu.set_opcode(std::make_shared<CPY>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xcd:
        cpu.set_opcode(std::make_shared<CMP>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xce:
        cpu.set_opcode(std::make_shared<DEC>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xcf:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xd0:
        cpu.set_opcode(std::make_shared<BNE>());
        cpu.set_mode(std::make_shared<REL>());
        break;
    case 0xd1:
        cpu.set_opcode(std::make_shared<CMP>());
        cpu.set_mode(std::make_shared<IDY>());
        break;
    case 0xd2:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xd3:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xd4:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0xd5:
        cpu.set_opcode(std::make_shared<CMP>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0xd6:
        cpu.set_opcode(std::make_shared<DEC>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0xd7:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xd8:
        cpu.set_opcode(std::make_shared<CLD>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xd9:
        cpu.set_opcode(std::make_shared<CMP>());
        cpu.set_mode(std::make_shared<ABY>());
        break;
    case 0xda:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xdb:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xdc:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0xdd:
        cpu.set_opcode(std::make_shared<CMP>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0xde:
        cpu.set_opcode(std::make_shared<DEC>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0xdf:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xe0:
        cpu.set_opcode(std::make_shared<CPX>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xe1:
        cpu.set_opcode(std::make_shared<SBC>());
        cpu.set_mode(std::make_shared<IDX>());
        break;
    case 0xe2:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xe3:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xe4:
        cpu.set_opcode(std::make_shared<CPX>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0xe5:
        cpu.set_opcode(std::make_shared<SBC>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0xe6:
        cpu.set_opcode(std::make_shared<INC>());
        cpu.set_mode(std::make_shared<ZPG>());
        break;
    case 0xe7:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xe8:
        cpu.set_opcode(std::make_shared<INX>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xe9:
        cpu.set_opcode(std::make_shared<SBC>());
        cpu.set_mode(std::make_shared<IMM>());
        break;
    case 0xea:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xeb:
        cpu.set_opcode(std::make_shared<SBC>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xec:
        cpu.set_opcode(std::make_shared<CPX>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xed:
        cpu.set_opcode(std::make_shared<SBC>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xee:
        cpu.set_opcode(std::make_shared<INC>());
        cpu.set_mode(std::make_shared<ABS>());
        break;
    case 0xef:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xf0:
        cpu.set_opcode(std::make_shared<BEQ>());
        cpu.set_mode(std::make_shared<REL>());
        break;
    case 0xf1:
        cpu.set_opcode(std::make_shared<SBC>());
        cpu.set_mode(std::make_shared<IDY>());
        break;
    case 0xf2:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xf3:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xf4:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0xf5:
        cpu.set_opcode(std::make_shared<SBC>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0xf6:
        cpu.set_opcode(std::make_shared<INC>());
        cpu.set_mode(std::make_shared<ZPX>());
        break;
    case 0xf7:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xf8:
        cpu.set_opcode(std::make_shared<SED>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xf9:
        cpu.set_opcode(std::make_shared<SBC>());
        cpu.set_mode(std::make_shared<ABY>());
        break;
    case 0xfa:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xfb:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    case 0xfc:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0xfd:
        cpu.set_opcode(std::make_shared<SBC>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0xfe:
        cpu.set_opcode(std::make_shared<INC>());
        cpu.set_mode(std::make_shared<ABX>());
        break;
    case 0xff:
        cpu.set_opcode(std::make_shared<NOP>());
        cpu.set_mode(std::make_shared<IMP>());
        break;
    }    
/*
    std::cout << std::hex << (int)r.pc - 1 << ' ' << cpu.get_instruction_id() << ' ' << cpu.get_mode_id()
        << "\tA:" << (int)r.a << " X:" << (int)r.x << " Y:" << 
        (int)r.y << " P:" << (int)r.sr << " SP:" << (int)r.sp << std::dec << " CYC: " << cpu.m_cycles
        << " Errors:" << (int)cpu.read(2) << std::endl;
    */
    cpu.set_state(std::make_shared<ExecuteState>());
    cpu.execute_state();
}

void ExecuteState::execute(CPU &cpu, CPURegister &r) const
{
    ++cpu.m_cycles;    
    if (cpu.step_mode())
        return;
    if (cpu.step_opcode())
        return;
    
    cpu.set_state(std::make_shared<ReadyState>());
}
