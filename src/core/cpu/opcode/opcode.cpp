#include "opcode.h"
#include "../cpu.h"

OpcodeTask ADC::execute(CPU &cpu, Register &r) const
{
    uint16_t temp = r.a + r.mdr + r.test(Register::StackRegister::C);

    r.set(Register::StackRegister::C, temp > 0x00FFu);
    r.set(Register::StackRegister::Z, (temp & 0x00FFu) == 0u);
    r.set(Register::StackRegister::V, ((temp ^ r.a) & ~(r.a ^ r.mdr)) & 0x0080u);
    r.set(Register::StackRegister::N, temp & 0x0080u);

    /* Wrte sum into A */
    r.a = temp & 0x00FFu;

    co_return 0;
}

OpcodeTask AND::execute(CPU &cpu, Register &r) const
{
    /* Perform AND with A and MDR and write to A */
    r.a &= r.mdr;
    r.set(Register::StackRegister::Z, r.a == 0x00u);
    r.set(Register::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask ASL::execute(CPU &cpu, Register &r) const
{
    if (cpu.get_mode_id() == "ABX" && r.mar >> 8u != r.mar - r.x >> 8u)
        co_yield -1; /* Re read from effective address */
    else if (cpu.get_mode_id() == "ABX" && r.mar >> 8u == r.mar - r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else if (cpu.get_mode_id() != "ACC")
        co_yield 0;

    if (cpu.get_mode_id() != "ACC")
        cpu.write(r.mdr, r.mar);

    r.set(Register::StackRegister::C, r.mdr & 0x80u);
    r.mdr <<= 1u;
    r.set(Register::StackRegister::Z, r.mdr == 0u);
    r.set(Register::StackRegister::N, r.mdr & 0x80u);

    if (cpu.get_mode_id() == "ACC")
    {
        /* A is set to MDR */
        r.a = r.mdr;

        co_return 0;
    }

    co_yield 0;

    cpu.write(r.mdr, r.mar);

    co_return 0;
}

OpcodeTask BCC::execute(CPU &cpu, Register &r) const
{
    if (r.test(Register::StackRegister::C) == false)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BCS::execute(CPU &cpu, Register &r) const
{
    if (r.test(Register::StackRegister::C) == true)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BEQ::execute(CPU &cpu, Register &r) const
{
    if (r.test(Register::StackRegister::Z) == true)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BIT::execute(CPU &cpu, Register &r) const
{
    /* Set Z if result is zero */
    r.set(Register::StackRegister::Z, !(r.a & r.mdr));

    /* Set V to Bit 6 */
    r.set(Register::StackRegister::V, r.mdr & 0x40u);

    /* Set N to Bit 7 */
    r.set(Register::StackRegister::N, r.mdr & 0x80u);

    co_return 0;
}

OpcodeTask BMI::execute(CPU &cpu, Register &r) const
{
    if (r.test(Register::StackRegister::N) == true)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BNE::execute(CPU &cpu, Register &r) const
{
    if (r.test(Register::StackRegister::Z) == false)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BPL::execute(CPU &cpu, Register &r) const
{
    if (r.test(Register::StackRegister::N) == false)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BRK::execute(CPU &cpu, Register &r) const
{
    /* Set B Flag of SR to true, and push incremented PCH into the stack*/
    cpu.write(r.pc >> 8u, 0x100u + r.sp--);
    co_yield 0;

    /* Push PCL into the stack*/
    cpu.write(r.pc & 0x00FFu, 0x100u + r.sp--);
    co_yield 0;

    /* Push P into the stack */
    r.set(Register::StackRegister::B, true);
    cpu.write(r.sr, 0x100u + r.sp--);
    r.set(Register::StackRegister::B, false);
    co_yield 0;

    r.pc = cpu.read(0xFFFEu);
    co_yield 0;

    r.pc |= cpu.read(0xFFFFu) << 8u;

    co_return 0;
}

OpcodeTask BVC::execute(CPU &cpu, Register &r) const
{
    if (r.test(Register::StackRegister::V) == false)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BVS::execute(CPU &cpu, Register &r) const
{
    if (r.test(Register::StackRegister::V) == true)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask CLC::execute(CPU &cpu, Register &r) const
{
    /* Clear Carry Flag */
    r.set(Register::StackRegister::C, false);

    co_return 0;
}

OpcodeTask CLD::execute(CPU &cpu, Register &r) const
{
    /* Clear Decimal Flag */
    r.set(Register::StackRegister::D, false);

    co_return 0;
}

OpcodeTask CLI::execute(CPU &cpu, Register &r) const
{
    /* Clear Interrupt Disabled Flag */
    r.set(Register::StackRegister::I, false);

    co_return 0;
}

OpcodeTask CLV::execute(CPU &cpu, Register &r) const
{
    /* Clear Overflow Flag */
    r.set(Register::StackRegister::V, false);

    co_return 0;
}

OpcodeTask CMP::execute(CPU &cpu, Register &r) const
{
    /* Set Flags */
    r.set(Register::StackRegister::C, r.a >= r.mdr);
    r.set(Register::StackRegister::Z, r.a == r.mdr);
    r.set(Register::StackRegister::N, (r.a - r.mdr) & 0x80u);

    co_return 0;
}

OpcodeTask CPX::execute(CPU &cpu, Register &r) const
{
    /* Set Flags */
    r.set(Register::StackRegister::C, r.x >= r.mdr);
    r.set(Register::StackRegister::Z, r.x == r.mdr);
    r.set(Register::StackRegister::N, (r.x - r.mdr) & 0x80u);

    co_return 0;
}

OpcodeTask CPY::execute(CPU &cpu, Register &r) const
{
    /* Set Flags */
    r.set(Register::StackRegister::C, r.y >= r.mdr);
    r.set(Register::StackRegister::Z, r.y == r.mdr);
    r.set(Register::StackRegister::N, (r.y - r.mdr) & 0x80u);

    co_return 0;
}

OpcodeTask DEC::execute(CPU &cpu, Register &r) const
{
    if (cpu.get_mode_id() == "ABX" && r.mar >> 8u != r.mar - r.x >> 8u)
        co_yield -1; /* Re-read from effective address */
    else if (cpu.get_mode_id() == "ABX" && r.mar >> 8u == r.mar - r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else
        co_yield 0;
    
    /* Decrement MDR */
    r.mdr -= 1u;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.mdr == 0u);
    r.set(Register::StackRegister::N, r.mdr & 0x80u);
    co_yield 0;

    /* Write MDR at address MAR */
    cpu.write(r.mdr, r.mar);

    co_return 0;
}

OpcodeTask DEX::execute(CPU &cpu, Register &r) const
{
    /* Decrement X */
    r.x -= 1u;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.x == 0u);
    r.set(Register::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask DEY::execute(CPU &cpu, Register &r) const
{
    /* Decrement Y */
    r.y -= 1u;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.y == 0u);
    r.set(Register::StackRegister::N, r.y & 0x80u);

    co_return 0;
}

OpcodeTask EOR::execute(CPU &cpu, Register &r) const
{
    /* Set A to A XOR MDR */
    r.a ^= r.mdr;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.a == 0u);
    r.set(Register::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask INC::execute(CPU &cpu, Register &r) const
{
    if (cpu.get_mode_id() == "ABX" && r.mar >> 8u != r.mar - r.x >> 8u)
        co_yield -1; /* Re-read from effective address */
    else if (cpu.get_mode_id() == "ABX" && r.mar >> 8u == r.mar - r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else
        co_yield 0;

    /* Increment MDR */
    r.mdr += 1;

    r.set(Register::StackRegister::Z, r.mdr == 0u);
    r.set(Register::StackRegister::N, r.mdr & 0x80u);
    co_yield 0;

    cpu.write(r.mdr, r.mar);

    co_return 0;
}

OpcodeTask INX::execute(CPU &cpu, Register &r) const
{
    /* Increment X */
    r.x += 1;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.x == 0u);
    r.set(Register::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask INY::execute(CPU &cpu, Register &r) const
{
    /* Increment Y */
    r.y += 1;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.y == 0u);
    r.set(Register::StackRegister::N, r.y & 0x80u);

    co_return 0;
}

OpcodeTask JMP::execute(CPU &cpu, Register &r) const
{
    /* Set PC to MAR */
    r.pc = r.mar;

    co_return 0;
}

OpcodeTask JSR::execute(CPU &cpu, Register &r) const
{
    /* Push Decremented PCH into stack */
    cpu.write(--r.pc >> 8u, 0x100u + r.sp--);
    co_yield 0;

    /* Push PCL into stack */
    cpu.write(r.pc & 0x00FFu, 0x100u + r.sp--);
    co_yield 0;

    /* Set PC to the Absolute Address */
    r.pc = r.mar;

    co_return 0;
}

OpcodeTask LDA::execute(CPU &cpu, Register &r) const
{
    /* Write contents of MDR to A */
    r.a = r.mdr;
    r.set(Register::StackRegister::Z, r.a == 0u);
    r.set(Register::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask LDX::execute(CPU &cpu, Register &r) const
{
    /* Write contents of MDR to X */
    r.x = r.mdr;
    r.set(Register::StackRegister::Z, r.x == 0u);
    r.set(Register::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask LDY::execute(CPU &cpu, Register &r) const
{
    /* Write contents of MDR to Y */
    r.y = r.mdr;
    r.set(Register::StackRegister::Z, r.y == 0u);
    r.set(Register::StackRegister::N, r.y & 0x80u);

    co_return 0;
}

OpcodeTask LSR::execute(CPU &cpu, Register &r) const
{
    if (cpu.get_mode_id() == "ABX" && r.mar >> 8u != r.mar - r.x >> 8u)
        co_yield -1; /* Re-read from effective address */
    else if (cpu.get_mode_id() == "ABX" && r.mar >> 8u == r.mar - r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else if (cpu.get_mode_id() != "ACC")
        co_yield 0;

    r.set(Register::StackRegister::C, r.mdr & 0x01u);
    r.mdr >>= 1u;
    r.set(Register::StackRegister::Z, r.mdr == 0u);
    r.set(Register::StackRegister::N, r.mdr & 0x80u);

    if (cpu.get_mode_id() != "ACC")
    {
        co_yield 0;

        cpu.write(r.mdr, r.mar);
    }
    else
        r.a = r.mdr;

    co_return 0;
}

OpcodeTask NOP::execute(CPU &cpu, Register &r) const
{
    /* Do Nothing */
    co_return 0;
}

OpcodeTask ORA::execute(CPU &cpu, Register &r) const
{
    /* Set A = A | MDR */
    r.a |= r.mdr; /* Push A into the stack */

    r.set(Register::StackRegister::Z, r.a == 0u);
    r.set(Register::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask PHA::execute(CPU &cpu, Register &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Push A into the stack */
    cpu.write(r.a, 0x100u + r.sp--);
    co_return 0;
}

OpcodeTask PHP::execute(CPU &cpu, Register &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Push P into the stack */
    cpu.write(r.sr, 0x100u + r.sp--);
    co_return 0;
}

OpcodeTask PLA::execute(CPU &cpu, Register &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment SP */
    ++r.sp;
    co_yield 0;

    /* Pull A from stack */
    r.a = cpu.read(0x100u + r.sp);

    r.set(Register::StackRegister::Z, r.a == 0u);
    r.set(Register::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask PLP::execute(CPU &cpu, Register &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment SP */
    ++r.sp;
    co_yield 0;

    /* Pull P from stack */
    r.mdr = r.sr;
    r.sr = cpu.read(0x100u + r.sp);

    r.set(Register::StackRegister::X, r.mdr & 0x20u);
    r.set(Register::StackRegister::B, r.mdr & 0x10u);

    co_return 0;
}

OpcodeTask ROL::execute(CPU &cpu, Register &r) const
{
    if (cpu.get_mode_id() == "ABX" && r.mar >> 8u != r.mar - r.x >> 8u)
        co_yield -1; /* Re-read from effective address */
    else if (cpu.get_mode_id() == "ABX" && r.mar >> 8u == r.mar - r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else if (cpu.get_mode_id() != "ACC")
        co_yield 0;

    auto C = r.mdr >> 7u;

    r.mdr = r.mdr << 1u | r.test(Register::StackRegister::C);

    r.set(Register::StackRegister::C, C);
    r.set(Register::StackRegister::Z, r.mdr == 0u);
    r.set(Register::StackRegister::N, r.mdr & 0x80u);

    if (cpu.get_mode_id() != "ACC")
    {
        co_yield 0;

        /* Write to effective address */
        cpu.write(r.mdr, r.mar);
    }
    else
        r.a = r.mdr;

    co_return 0;
}

OpcodeTask ROR::execute(CPU &cpu, Register &r) const
{
    if (cpu.get_mode_id() == "ABX" && r.mar >> 8u != r.mar - r.x >> 8u)
        co_yield -1;
    else if (cpu.get_mode_id() == "ABX" && r.mar >> 8u == r.mar - r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else if (cpu.get_mode_id() != "ACC")
        co_yield 0;

    auto C = r.mdr & 0x0001u;

    r.mdr = r.mdr >> 1u | r.test(Register::StackRegister::C) << 7u;

    r.set(Register::StackRegister::C, C);
    r.set(Register::StackRegister::Z, r.mdr == 0u);
    r.set(Register::StackRegister::N, r.mdr >> 7u);

    if (cpu.get_mode_id() != "ACC")
    {
        co_yield 0;

        /* Write to effective address */
        cpu.write(r.mdr, r.mar);
    }
    else
        r.a = r.mdr;

    co_return 0;
}

OpcodeTask RTI::execute(CPU &cpu, Register &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment Stack Pointer */
    ++r.sp;
    co_yield 0;

    /* Pull P from the Stack */
    r.mdr = r.sr;
    r.sr = cpu.read(0x100u + r.sp);

    r.set(Register::StackRegister::X, r.mdr & 0x20u);
    r.set(Register::StackRegister::B, r.mdr & 0x10u);
    co_yield 0;

    /* Pull PCL from the Stack */
    r.pc = cpu.read(0x100u + ++r.sp);
    co_yield 0;

    /* Pull PCH from the Stack */
    r.pc |= cpu.read(0x100u + ++r.sp) << 8u;

    co_return 0;
}

OpcodeTask RTS::execute(CPU &cpu, Register &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment SP */
    r.sp++;
    co_yield 0;

    /* Pull PCL from stack */
    r.pc = cpu.read(0x100u + r.sp++);
    co_yield 0;

    /* Pull PCH from stack */
    r.pc |= cpu.read(0x100u + r.sp) << 8u;
    co_yield 0;

    /* Increment PC */
    ++r.pc;
    co_return 0;
}

OpcodeTask SBC::execute(CPU &cpu, Register &r) const
{
    /* Invert bits of MDR */
    uint16_t value = r.mdr ^ 0x00FFu;

    uint16_t temp = r.a + value + r.test(Register::StackRegister::C);

    r.set(Register::StackRegister::C, temp > 0x00FFu);
    r.set(Register::StackRegister::Z, (temp & 0x00FFu) == 0u);
    r.set(Register::StackRegister::V, ((temp ^ r.a) & (r.a ^ r.mdr)) & 0x0080u);
    r.set(Register::StackRegister::N, temp & 0x0080u);

    r.a = temp & 0x00FFu;

    co_return 0;
}

OpcodeTask SEC::execute(CPU &cpu, Register &r) const
{
    /* Set Carry Flag to True */
    r.set(Register::StackRegister::C, true);

    co_return 0;
}

OpcodeTask SED::execute(CPU &cpu, Register &r) const
{
    /* Set Decimal Flag to True */
    r.set(Register::StackRegister::D, true);

    co_return 0;
}

OpcodeTask SEI::execute(CPU &cpu, Register &r) const
{
    /* Set Interrupt Disable Flag to True */
    r.set(Register::StackRegister::I, true);

    co_return 0;
}

OpcodeTask STA::execute(CPU &cpu, Register &r) const
{
    /* Take an extra cycle if high byte was not fixed */
    if (cpu.get_mode_id() == "IDY" && (r.mar >> 8u == r.mar - r.y >> 8u))
        co_yield -1;
    else if (cpu.get_mode_id() == "ABX" && (r.mar >> 8u == r.mar - r.x >> 8u))
        co_yield -1;
    else if (cpu.get_mode_id() == "ABY" && (r.mar >> 8u == r.mar - r.y >> 8u))
        co_yield -1;

    /* Store A at MAR */
    cpu.write(r.a, r.mar);

    co_return 0;
}

OpcodeTask STX::execute(CPU &cpu, Register &r) const
{
    /* Store X at MAR */
    cpu.write(r.x, r.mar);

    co_return 0;
}

OpcodeTask STY::execute(CPU &cpu, Register &r) const
{
    /* Store X at MAR */
    cpu.write(r.y, r.mar);

    co_return 0;
}

OpcodeTask TAX::execute(CPU &cpu, Register &r) const
{
    /* Transfer A to X */
    r.x = r.a;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.x == 0u);
    r.set(Register::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask TAY::execute(CPU &cpu, Register &r) const
{
    /* Transfer A to Y */
    r.y = r.a;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.y == 0u);
    r.set(Register::StackRegister::N, r.y & 0x80u);

    co_return 0;
}

OpcodeTask TSX::execute(CPU &cpu, Register &r) const
{
    /* Transfer SP to X */
    r.x = r.sp;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.x == 0u);
    r.set(Register::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask TXA::execute(CPU &cpu, Register &r) const
{
    /* Transfer X to A */
    r.a = r.x;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.a == 0u);
    r.set(Register::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask TXS::execute(CPU &cpu, Register &r) const
{
    /* Transfer X to SP */
    r.sp = r.x;

    co_return 0;
}

OpcodeTask TYA::execute(CPU &cpu, Register &r) const
{
    r.a = r.y;

    /* Set Flags */
    r.set(Register::StackRegister::Z, r.a == 0u);
    r.set(Register::StackRegister::N, r.a & 0x80u);

    co_return 0;
}
