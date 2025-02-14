#include "cpu.h"

bool CPU::ADC()
{
    uint16_t temp = s.r.a + s.r.mdr + s.r.test(CPURegister::StackRegister::C);

    s.r.set(CPURegister::StackRegister::C, temp > 0x00FFu);
    s.r.set(CPURegister::StackRegister::Z, (temp & 0x00FFu) == 0u);
    s.r.set(CPURegister::StackRegister::V, ((temp ^ s.r.a) & ~(s.r.a ^ s.r.mdr)) & 0x0080u);
    s.r.set(CPURegister::StackRegister::N, temp & 0x0080u);

    /* Wrte sum into A */
    s.r.a = temp & 0x00FFu;

    co_return 0;
}

bool CPU::AND()
{
    /* Perform AND with A and MDR and write to A */
    s.r.a &= s.r.mdr;
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0x00u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    co_return 0;
}

bool CPU::ASL()
{
    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        co_yield -1; /* Re read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else if (s.mode != CPUMode::ACC)
        co_yield 0;

    if (s.mode != CPUMode::ACC)
        bus.cpu_write(s.r.mdr, s.r.mar);

    s.r.set(CPURegister::StackRegister::C, s.r.mdr & 0x80u);
    s.r.mdr <<= 1u;
    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);

    if (s.mode == CPUMode::ACC)
    {
        /* A is set to MDR */
        s.r.a = s.r.mdr;

        co_return 0;
    }

    co_yield 0;

    bus.cpu_write(s.r.mdr, s.r.mar);

    co_return 0;
}

bool CPU::BCC()
{
    if (s.r.test(CPURegister::StackRegister::C) == false)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        co_yield 1;

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

bool CPU::BCS()
{
    if (s.r.test(CPURegister::StackRegister::C) == true)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        co_yield 1;

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

bool CPU::BEQ()
{
    if (s.r.test(CPURegister::StackRegister::Z) == true)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        co_yield 1;

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

bool CPU::BIT()
{
    /* Set Z if result is zero */
    s.r.set(CPURegister::StackRegister::Z, !(s.r.a & s.r.mdr));

    /* Set V to Bit 6 */
    s.r.set(CPURegister::StackRegister::V, s.r.mdr & 0x40u);

    /* Set N to Bit 7 */
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);

    co_return 0;
}

bool CPU::BMI()
{
    if (s.r.test(CPURegister::StackRegister::N) == true)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        co_yield 1;

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

bool CPU::BNE()
{
    if (s.r.test(CPURegister::StackRegister::Z) == false)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        co_yield 1;

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

bool CPU::BPL()
{
    if (s.r.test(CPURegister::StackRegister::N) == false)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        co_yield 1;

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

bool CPU::BRK()
{
    /* Set B Flag of SR to true, and push incremented PCH into the stack*/
    bus.cpu_write(s.r.pc >> 8u, 0x100u + s.r.sp--);
    co_yield 0;

    /* Push PCL into the stack*/
    bus.cpu_write(s.r.pc & 0x00FFu, 0x100u + s.r.sp--);
    co_yield 0;

    /* Push P into the stack */
    s.r.set(CPURegister::StackRegister::B, true);
    bus.cpu_write(s.r.sr, 0x100u + s.r.sp--);
    s.r.set(CPURegister::StackRegister::B, false);
    co_yield 0;

    s.r.pc = bus.cpu_read(0xFFFEu);
    co_yield 0;

    s.r.pc |= bus.cpu_read(0xFFFFu) << 8u;

    co_return 0;
}

bool CPU::BVC()
{
    if (s.r.test(CPURegister::StackRegister::V) == false)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        co_yield 1;

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

bool CPU::BVS()
{
    if (s.r.test(CPURegister::StackRegister::V) == true)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        co_yield 1;

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            co_yield 1;
    }

    co_return 0;
}

bool CPU::CLC()
{
    /* Clear Carry Flag */
    s.r.set(CPURegister::StackRegister::C, false);

    co_return 0;
}

bool CPU::CLD()
{
    /* Clear Decimal Flag */
    s.r.set(CPURegister::StackRegister::D, false);

    co_return 0;
}

bool CPU::CLI()
{
    /* Clear Interrupt Disabled Flag */
    s.r.set(CPURegister::StackRegister::I, false);

    co_return 0;
}

bool CPU::CLV()
{
    /* Clear Overflow Flag */
    s.r.set(CPURegister::StackRegister::V, false);

    co_return 0;
}

bool CPU::CMP()
{
    /* Set Flags */
    s.r.set(CPURegister::StackRegister::C, s.r.a >= s.r.mdr);
    s.r.set(CPURegister::StackRegister::Z, s.r.a == s.r.mdr);
    s.r.set(CPURegister::StackRegister::N, (s.r.a - s.r.mdr) & 0x80u);

    co_return 0;
}

bool CPU::CPX()
{
    /* Set Flags */
    s.r.set(CPURegister::StackRegister::C, s.r.x >= s.r.mdr);
    s.r.set(CPURegister::StackRegister::Z, s.r.x == s.r.mdr);
    s.r.set(CPURegister::StackRegister::N, (s.r.x - s.r.mdr) & 0x80u);

    co_return 0;
}

bool CPU::CPY()
{
    /* Set Flags */
    s.r.set(CPURegister::StackRegister::C, s.r.y >= s.r.mdr);
    s.r.set(CPURegister::StackRegister::Z, s.r.y == s.r.mdr);
    s.r.set(CPURegister::StackRegister::N, (s.r.y - s.r.mdr) & 0x80u);

    co_return 0;
}

bool CPU::DEC()
{
    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        co_yield -1; /* Re-read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else
        co_yield 0;
    
    /* Decrement MDR */
    s.r.mdr -= 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);
    co_yield 0;

    /* Write MDR at address MAR */
    bus.cpu_write(s.r.mdr, s.r.mar);

    co_return 0;
}

bool CPU::DEX()
{
    /* Decrement X */
    s.r.x -= 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    co_return 0;
}

bool CPU::DEY()
{
    /* Decrement Y */
    s.r.y -= 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.y == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.y & 0x80u);

    co_return 0;
}

bool CPU::EOR()
{
    /* Set A to A XOR MDR */
    s.r.a ^= s.r.mdr;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    co_return 0;
}

bool CPU::INC()
{
    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        co_yield -1; /* Re-read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else
        co_yield 0;

    /* Increment MDR */
    s.r.mdr += 1u;

    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);
    co_yield 0;

    bus.cpu_write(s.r.mdr, s.r.mar);

    co_return 0;
}

bool CPU::INX()
{
    /* Increment X */
    s.r.x += 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    co_return 0;
}

bool CPU::INY()
{
    /* Increment Y */
    s.r.y += 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.y == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.y & 0x80u);

    co_return 0;
}

bool CPU::JMP()
{
    /* Set PC to MAR */
    s.r.pc = s.r.mar;

    co_return 0;
}

bool CPU::JSR()
{
    /* Push Decremented PCH into stack */
    bus.cpu_write(--s.r.pc >> 8u, 0x100u + s.r.sp--);
    co_yield 0;

    /* Push PCL into stack */
    bus.cpu_write(s.r.pc & 0x00FFu, 0x100u + s.r.sp--);
    co_yield 0;

    /* Set PC to the Absolute Address */
    s.r.pc = s.r.mar;

    co_return 0;
}

bool CPU::LDA()
{
    /* Write contents of MDR to A */
    s.r.a = s.r.mdr;
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    co_return 0;
}

bool CPU::LDX()
{
    /* Write contents of MDR to X */
    s.r.x = s.r.mdr;
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    co_return 0;
}

bool CPU::LDY()
{
    /* Write contents of MDR to Y */
    s.r.y = s.r.mdr;
    s.r.set(CPURegister::StackRegister::Z, s.r.y == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.y & 0x80u);

    co_return 0;
}

bool CPU::LSR()
{
    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        co_yield -1; /* Re-read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else if (s.mode != CPUMode::ACC)
        co_yield 0;

    s.r.set(CPURegister::StackRegister::C, s.r.mdr & 0x01u);
    s.r.mdr >>= 1u;
    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);

    if (s.mode != CPUMode::ACC)
    {
        co_yield 0;

        bus.cpu_write(s.r.mdr, s.r.mar);
    }
    else
        s.r.a = s.r.mdr;

    co_return 0;
}

bool CPU::NOP()
{
    /* Do Nothing */
    co_return 0;
}

bool CPU::ORA()
{
    /* Set A = A | MDR */
    s.r.a |= s.r.mdr; /* Push A into the stack */

    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    co_return 0;
}

bool CPU::PHA()
{
    /* Tick after dummy read */
    co_yield 0;

    /* Push A into the stack */
    bus.cpu_write(s.r.a, 0x100u + s.r.sp--);
    co_return 0;
}

bool CPU::PHP()
{
    /* Tick after dummy read */
    co_yield 0;

    /* Push P into the stack */
    bus.cpu_write(s.r.sr, 0x100u + s.r.sp--);
    co_return 0;
}

bool CPU::PLA()
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment SP */
    ++s.r.sp;
    co_yield 0;

    /* Pull A from stack */
    s.r.a = bus.cpu_read(0x100u + s.r.sp);

    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    co_return 0;
}

bool CPU::PLP()
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment SP */
    ++s.r.sp;
    co_yield 0;

    /* Pull P from stack */
    s.r.mdr = s.r.sr;
    s.r.sr = bus.cpu_read(0x100u + s.r.sp);

    s.r.set(CPURegister::StackRegister::X, s.r.mdr & 0x20u);
    s.r.set(CPURegister::StackRegister::B, s.r.mdr & 0x10u);

    co_return 0;
}

bool CPU::ROL()
{
    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        co_yield -1; /* Re-read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else if (s.mode != CPUMode::ACC)
        co_yield 0;

    auto C = s.r.mdr >> 7u;

    s.r.mdr = s.r.mdr << 1u | s.r.test(CPURegister::StackRegister::C);

    s.r.set(CPURegister::StackRegister::C, C);
    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);

    if (s.mode != CPUMode::ACC)
    {
        co_yield 0;

        /* Write to effective address */
        bus.cpu_write(s.r.mdr, s.r.mar);
    }
    else
        s.r.a = s.r.mdr;

    co_return 0;
}

bool CPU::ROR()
{
    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        co_yield -1;
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        co_yield 0;

        /* Re-read from effective address */
        co_yield 0;
    }
    else if (s.mode != CPUMode::ACC)
        co_yield 0;

    auto C = s.r.mdr & 0x0001u;

    s.r.mdr = s.r.mdr >> 1u | s.r.test(CPURegister::StackRegister::C) << 7u;

    s.r.set(CPURegister::StackRegister::C, C);
    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr >> 7u);

    if (s.mode != CPUMode::ACC)
    {
        co_yield 0;

        /* Write to effective address */
        bus.cpu_write(s.r.mdr, s.r.mar);
    }
    else
        s.r.a = s.r.mdr;

    co_return 0;
}

bool CPU::RTI()
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment Stack Pointer */
    ++s.r.sp;
    co_yield 0;

    /* Pull P from the Stack */
    s.r.mdr = s.r.sr;
    s.r.sr = bus.cpu_read(0x100u + s.r.sp);

    s.r.set(CPURegister::StackRegister::X, s.r.mdr & 0x20u);
    s.r.set(CPURegister::StackRegister::B, s.r.mdr & 0x10u);
    co_yield 0;

    /* Pull PCL from the Stack */
    s.r.pc = bus.cpu_read(0x100u + ++s.r.sp);
    co_yield 0;

    /* Pull PCH from the Stack */
    s.r.pc |= bus.cpu_read(0x100u + ++s.r.sp) << 8u;

    co_return 0;
}

bool CPU::RTS()
{
    /* Tick after dummy read */
    co_yield 0;

    /* Increment SP */
    s.r.sp++;
    co_yield 0;

    /* Pull PCL from stack */
    s.r.pc = bus.cpu_read(0x100u + s.r.sp++);
    co_yield 0;

    /* Pull PCH from stack */
    s.r.pc |= bus.cpu_read(0x100u + s.r.sp) << 8u;
    co_yield 0;

    /* Increment PC */
    ++s.r.pc;
    co_return 0;
}

bool CPU::SBC()
{
    /* Invert bits of MDR */
    uint16_t value = s.r.mdr ^ 0x00FFu;

    uint16_t temp = s.r.a + value + s.r.test(CPURegister::StackRegister::C);

    s.r.set(CPURegister::StackRegister::C, temp > 0x00FFu);
    s.r.set(CPURegister::StackRegister::Z, (temp & 0x00FFu) == 0u);
    s.r.set(CPURegister::StackRegister::V, ((temp ^ s.r.a) & (s.r.a ^ s.r.mdr)) & 0x0080u);
    s.r.set(CPURegister::StackRegister::N, temp & 0x0080u);

    s.r.a = temp & 0x00FFu;

    co_return 0;
}

bool CPU::SEC()
{
    /* Set Carry Flag to True */
    s.r.set(CPURegister::StackRegister::C, true);

    co_return 0;
}

bool CPU::SED()
{
    /* Set Decimal Flag to True */
    s.r.set(CPURegister::StackRegister::D, true);

    co_return 0;
}

bool CPU::SEI()
{
    /* Set Interrupt Disable Flag to True */
    s.r.set(CPURegister::StackRegister::I, true);

    co_return 0;
}

bool CPU::STA()
{
    /* Take an extra cycle if high byte was not fixed */
    if (s.mode == CPUMode::IDY && (s.r.mar >> 8u == s.r.mar - s.r.y >> 8u))
        co_yield -1;
    else if (s.mode == CPUMode::ABX && (s.r.mar >> 8u == s.r.mar - s.r.x >> 8u))
        co_yield -1;
    else if (s.mode == CPUMode::ABY && (s.r.mar >> 8u == s.r.mar - s.r.y >> 8u))
        co_yield -1;

    /* Store A at MAR */
    bus.cpu_write(s.r.a, s.r.mar);

    co_return 0;
}

bool CPU::STX()
{
    /* Store X at MAR */
    bus.cpu_write(s.r.x, s.r.mar);

    co_return 0;
}

bool CPU::STY()
{
    /* Store X at MAR */
    bus.cpu_write(s.r.y, s.r.mar);

    co_return 0;
}

bool CPU::TAX()
{
    /* Transfer A to X */
    s.r.x = s.r.a;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    co_return 0;
}

bool CPU::TAY()
{
    /* Transfer A to Y */
    s.r.y = s.r.a;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.y == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.y & 0x80u);

    co_return 0;
}

bool CPU::TSX()
{
    /* Transfer SP to X */
    s.r.x = s.r.sp;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    co_return 0;
}

bool CPU::TXA()
{
    /* Transfer X to A */
    s.r.a = s.r.x;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    co_return 0;
}

bool CPU::TXS()
{
    /* Transfer X to SP */
    s.r.sp = s.r.x;

    co_return 0;
}

bool CPU::TYA()
{
    s.r.a = s.r.y;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    co_return 0;
}