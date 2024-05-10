#include "opcode.h"
#include "cpu.h"

OpcodeTask ADC::execute(CPU &cpu, CPURegister &r) const
{
    uint16_t temp = r.a + r.mdr + r.test(CPURegister::StackRegister::C);

    r.set(CPURegister::StackRegister::C, temp > 0x00FFu);
    r.set(CPURegister::StackRegister::Z, (temp & 0x00FFu) == 0u);
    r.set(CPURegister::StackRegister::V, ((temp ^ r.a) & ~(r.a ^ r.mdr)) & 0x0080u);
    r.set(CPURegister::StackRegister::N, temp & 0x0080u);

    /* Wrte sum into A */
    r.a = temp & 0x00FFu;

    co_return 0;
}

OpcodeTask AND::execute(CPU &cpu, CPURegister &r) const
{
    /* Perform AND with A and MDR and write to A */
    r.a &= r.mdr;
    r.set(CPURegister::StackRegister::Z, r.a == 0x00u);
    r.set(CPURegister::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask ASL::execute(CPU &cpu, CPURegister &r) const
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

    r.set(CPURegister::StackRegister::C, r.mdr & 0x80u);
    r.mdr <<= 1u;
    r.set(CPURegister::StackRegister::Z, r.mdr == 0u);
    r.set(CPURegister::StackRegister::N, r.mdr & 0x80u);

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

OpcodeTask BCC::execute(CPU &cpu, CPURegister &r) const
{
    if (r.test(CPURegister::StackRegister::C) == false)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BCS::execute(CPU &cpu, CPURegister &r) const
{
    if (r.test(CPURegister::StackRegister::C) == true)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BEQ::execute(CPU &cpu, CPURegister &r) const
{
    if (r.test(CPURegister::StackRegister::Z) == true)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BIT::execute(CPU &cpu, CPURegister &r) const
{
    /* Set Z if result is zero */
    r.set(CPURegister::StackRegister::Z, !(r.a & r.mdr));

    /* Set V to Bit 6 */
    r.set(CPURegister::StackRegister::V, r.mdr & 0x40u);

    /* Set N to Bit 7 */
    r.set(CPURegister::StackRegister::N, r.mdr & 0x80u);

    co_return 0;
}

OpcodeTask BMI::execute(CPU &cpu, CPURegister &r) const
{
    if (r.test(CPURegister::StackRegister::N) == true)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BNE::execute(CPU &cpu, CPURegister &r) const
{
    if (r.test(CPURegister::StackRegister::Z) == false)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BPL::execute(CPU &cpu, CPURegister &r) const
{
    if (r.test(CPURegister::StackRegister::N) == false)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BRK::execute(CPU &cpu, CPURegister &r) const
{
    /* Set B Flag of SR to true, and push incremented PCH into the stack*/
    cpu.write(r.pc >> 8u, 0x100u + r.sp--);
    co_yield 0;

    /* Push PCL into the stack*/
    cpu.write(r.pc & 0x00FFu, 0x100u + r.sp--);
    co_yield 0;

    /* Push P into the stack */
    r.set(CPURegister::StackRegister::B, true);
    cpu.write(r.sr, 0x100u + r.sp--);
    r.set(CPURegister::StackRegister::B, false);
    co_yield 0;

    r.pc = cpu.read(0xFFFEu);
    co_yield 0;

    r.pc |= cpu.read(0xFFFFu) << 8u;

    co_return 0;
}

OpcodeTask BVC::execute(CPU &cpu, CPURegister &r) const
{
    if (r.test(CPURegister::StackRegister::V) == false)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask BVS::execute(CPU &cpu, CPURegister &r) const
{
    if (r.test(CPURegister::StackRegister::V) == true)
    {
        r.pc += static_cast<int8_t>(r.mdr);
        co_yield 1;

        if (r.pc >> 8u != r.pc - static_cast<int8_t>(r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

OpcodeTask CLC::execute(CPU &cpu, CPURegister &r) const
{
    /* Clear Carry Flag */
    r.set(CPURegister::StackRegister::C, false);

    co_return 0;
}

OpcodeTask CLD::execute(CPU &cpu, CPURegister &r) const
{
    /* Clear Decimal Flag */
    r.set(CPURegister::StackRegister::D, false);

    co_return 0;
}

OpcodeTask CLI::execute(CPU &cpu, CPURegister &r) const
{
    /* Clear Interrupt Disabled Flag */
    r.set(CPURegister::StackRegister::I, false);

    co_return 0;
}

OpcodeTask CLV::execute(CPU &cpu, CPURegister &r) const
{
    /* Clear Overflow Flag */
    r.set(CPURegister::StackRegister::V, false);

    co_return 0;
}

OpcodeTask CMP::execute(CPU &cpu, CPURegister &r) const
{
    /* Set Flags */
    r.set(CPURegister::StackRegister::C, r.a >= r.mdr);
    r.set(CPURegister::StackRegister::Z, r.a == r.mdr);
    r.set(CPURegister::StackRegister::N, (r.a - r.mdr) & 0x80u);

    co_return 0;
}

OpcodeTask CPX::execute(CPU &cpu, CPURegister &r) const
{
    /* Set Flags */
    r.set(CPURegister::StackRegister::C, r.x >= r.mdr);
    r.set(CPURegister::StackRegister::Z, r.x == r.mdr);
    r.set(CPURegister::StackRegister::N, (r.x - r.mdr) & 0x80u);

    co_return 0;
}

OpcodeTask CPY::execute(CPU &cpu, CPURegister &r) const
{
    /* Set Flags */
    r.set(CPURegister::StackRegister::C, r.y >= r.mdr);
    r.set(CPURegister::StackRegister::Z, r.y == r.mdr);
    r.set(CPURegister::StackRegister::N, (r.y - r.mdr) & 0x80u);

    co_return 0;
}

OpcodeTask DEC::execute(CPU &cpu, CPURegister &r) const
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
    r.set(CPURegister::StackRegister::Z, r.mdr == 0u);
    r.set(CPURegister::StackRegister::N, r.mdr & 0x80u);
    co_yield 0;

    /* Write MDR at address MAR */
    cpu.write(r.mdr, r.mar);

    co_return 0;
}

OpcodeTask DEX::execute(CPU &cpu, CPURegister &r) const
{
    /* Decrement X */
    r.x -= 1u;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.x == 0u);
    r.set(CPURegister::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask DEY::execute(CPU &cpu, CPURegister &r) const
{
    /* Decrement Y */
    r.y -= 1u;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.y == 0u);
    r.set(CPURegister::StackRegister::N, r.y & 0x80u);

    co_return 0;
}

OpcodeTask EOR::execute(CPU &cpu, CPURegister &r) const
{
    /* Set A to A XOR MDR */
    r.a ^= r.mdr;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.a == 0u);
    r.set(CPURegister::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask INC::execute(CPU &cpu, CPURegister &r) const
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

    r.set(CPURegister::StackRegister::Z, r.mdr == 0u);
    r.set(CPURegister::StackRegister::N, r.mdr & 0x80u);
    co_yield 0;

    cpu.write(r.mdr, r.mar);

    co_return 0;
}

OpcodeTask INX::execute(CPU &cpu, CPURegister &r) const
{
    /* Increment X */
    r.x += 1;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.x == 0u);
    r.set(CPURegister::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask INY::execute(CPU &cpu, CPURegister &r) const
{
    /* Increment Y */
    r.y += 1;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.y == 0u);
    r.set(CPURegister::StackRegister::N, r.y & 0x80u);

    co_return 0;
}

OpcodeTask JMP::execute(CPU &cpu, CPURegister &r) const
{
    /* Set PC to MAR */
    r.pc = r.mar;

    co_return 0;
}

OpcodeTask JSR::execute(CPU &cpu, CPURegister &r) const
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

OpcodeTask LDA::execute(CPU &cpu, CPURegister &r) const
{
    /* Write contents of MDR to A */
    r.a = r.mdr;
    r.set(CPURegister::StackRegister::Z, r.a == 0u);
    r.set(CPURegister::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask LDX::execute(CPU &cpu, CPURegister &r) const
{
    /* Write contents of MDR to X */
    r.x = r.mdr;
    r.set(CPURegister::StackRegister::Z, r.x == 0u);
    r.set(CPURegister::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask LDY::execute(CPU &cpu, CPURegister &r) const
{
    /* Write contents of MDR to Y */
    r.y = r.mdr;
    r.set(CPURegister::StackRegister::Z, r.y == 0u);
    r.set(CPURegister::StackRegister::N, r.y & 0x80u);

    co_return 0;
}

OpcodeTask LSR::execute(CPU &cpu, CPURegister &r) const
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

    r.set(CPURegister::StackRegister::C, r.mdr & 0x01u);
    r.mdr >>= 1u;
    r.set(CPURegister::StackRegister::Z, r.mdr == 0u);
    r.set(CPURegister::StackRegister::N, r.mdr & 0x80u);

    if (cpu.get_mode_id() != "ACC")
    {
        co_yield 0;

        cpu.write(r.mdr, r.mar);
    }
    else
        r.a = r.mdr;

    co_return 0;
}

OpcodeTask NOP::execute(CPU &cpu, CPURegister &r) const
{
    /* Do Nothing */
    co_return 0;
}

OpcodeTask ORA::execute(CPU &cpu, CPURegister &r) const
{
    /* Set A = A | MDR */
    r.a |= r.mdr; /* Push A into the stack */

    r.set(CPURegister::StackRegister::Z, r.a == 0u);
    r.set(CPURegister::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask PHA::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Push A into the stack */
    cpu.write(r.a, 0x100u + r.sp--);
    co_return 0;
}

OpcodeTask PHP::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Push P into the stack */
    cpu.write(r.sr, 0x100u + r.sp--);
    co_return 0;
}

OpcodeTask PLA::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment SP */
    ++r.sp;
    co_yield 0;

    /* Pull A from stack */
    r.a = cpu.read(0x100u + r.sp);

    r.set(CPURegister::StackRegister::Z, r.a == 0u);
    r.set(CPURegister::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask PLP::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment SP */
    ++r.sp;
    co_yield 0;

    /* Pull P from stack */
    r.mdr = r.sr;
    r.sr = cpu.read(0x100u + r.sp);

    r.set(CPURegister::StackRegister::X, r.mdr & 0x20u);
    r.set(CPURegister::StackRegister::B, r.mdr & 0x10u);

    co_return 0;
}

OpcodeTask ROL::execute(CPU &cpu, CPURegister &r) const
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

    r.mdr = r.mdr << 1u | r.test(CPURegister::StackRegister::C);

    r.set(CPURegister::StackRegister::C, C);
    r.set(CPURegister::StackRegister::Z, r.mdr == 0u);
    r.set(CPURegister::StackRegister::N, r.mdr & 0x80u);

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

OpcodeTask ROR::execute(CPU &cpu, CPURegister &r) const
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

    r.mdr = r.mdr >> 1u | r.test(CPURegister::StackRegister::C) << 7u;

    r.set(CPURegister::StackRegister::C, C);
    r.set(CPURegister::StackRegister::Z, r.mdr == 0u);
    r.set(CPURegister::StackRegister::N, r.mdr >> 7u);

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

OpcodeTask RTI::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment Stack Pointer */
    ++r.sp;
    co_yield 0;

    /* Pull P from the Stack */
    r.mdr = r.sr;
    r.sr = cpu.read(0x100u + r.sp);

    r.set(CPURegister::StackRegister::X, r.mdr & 0x20u);
    r.set(CPURegister::StackRegister::B, r.mdr & 0x10u);
    co_yield 0;

    /* Pull PCL from the Stack */
    r.pc = cpu.read(0x100u + ++r.sp);
    co_yield 0;

    /* Pull PCH from the Stack */
    r.pc |= cpu.read(0x100u + ++r.sp) << 8u;

    co_return 0;
}

OpcodeTask RTS::execute(CPU &cpu, CPURegister &r) const
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

OpcodeTask SBC::execute(CPU &cpu, CPURegister &r) const
{
    /* Invert bits of MDR */
    uint16_t value = r.mdr ^ 0x00FFu;

    uint16_t temp = r.a + value + r.test(CPURegister::StackRegister::C);

    r.set(CPURegister::StackRegister::C, temp > 0x00FFu);
    r.set(CPURegister::StackRegister::Z, (temp & 0x00FFu) == 0u);
    r.set(CPURegister::StackRegister::V, ((temp ^ r.a) & (r.a ^ r.mdr)) & 0x0080u);
    r.set(CPURegister::StackRegister::N, temp & 0x0080u);

    r.a = temp & 0x00FFu;

    co_return 0;
}

OpcodeTask SEC::execute(CPU &cpu, CPURegister &r) const
{
    /* Set Carry Flag to True */
    r.set(CPURegister::StackRegister::C, true);

    co_return 0;
}

OpcodeTask SED::execute(CPU &cpu, CPURegister &r) const
{
    /* Set Decimal Flag to True */
    r.set(CPURegister::StackRegister::D, true);

    co_return 0;
}

OpcodeTask SEI::execute(CPU &cpu, CPURegister &r) const
{
    /* Set Interrupt Disable Flag to True */
    r.set(CPURegister::StackRegister::I, true);

    co_return 0;
}

OpcodeTask STA::execute(CPU &cpu, CPURegister &r) const
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

OpcodeTask STX::execute(CPU &cpu, CPURegister &r) const
{
    /* Store X at MAR */
    cpu.write(r.x, r.mar);

    co_return 0;
}

OpcodeTask STY::execute(CPU &cpu, CPURegister &r) const
{
    /* Store X at MAR */
    cpu.write(r.y, r.mar);

    co_return 0;
}

OpcodeTask TAX::execute(CPU &cpu, CPURegister &r) const
{
    /* Transfer A to X */
    r.x = r.a;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.x == 0u);
    r.set(CPURegister::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask TAY::execute(CPU &cpu, CPURegister &r) const
{
    /* Transfer A to Y */
    r.y = r.a;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.y == 0u);
    r.set(CPURegister::StackRegister::N, r.y & 0x80u);

    co_return 0;
}

OpcodeTask TSX::execute(CPU &cpu, CPURegister &r) const
{
    /* Transfer SP to X */
    r.x = r.sp;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.x == 0u);
    r.set(CPURegister::StackRegister::N, r.x & 0x80u);

    co_return 0;
}

OpcodeTask TXA::execute(CPU &cpu, CPURegister &r) const
{
    /* Transfer X to A */
    r.a = r.x;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.a == 0u);
    r.set(CPURegister::StackRegister::N, r.a & 0x80u);

    co_return 0;
}

OpcodeTask TXS::execute(CPU &cpu, CPURegister &r) const
{
    /* Transfer X to SP */
    r.sp = r.x;

    co_return 0;
}

OpcodeTask TYA::execute(CPU &cpu, CPURegister &r) const
{
    r.a = r.y;

    /* Set Flags */
    r.set(CPURegister::StackRegister::Z, r.a == 0u);
    r.set(CPURegister::StackRegister::N, r.a & 0x80u);

    co_return 0;
}
