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

    return true;
}

bool CPU::AND()
{
    /* Perform AND with A and MDR and write to A */
    s.r.a &= s.r.mdr;
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0x00u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    return true;
}

bool CPU::ASL()
{
    INIT_SEGMENTS

    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        BREAK_SEGMENT(1) /* Re read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else if (s.mode != CPUMode::ACC)
        BREAK_SEGMENT(4)

    START_SEGMENT(5)

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

        FREE_SEGMENTS
        return true;
    }

    END_SEGMENT

    bus.cpu_write(s.r.mdr, s.r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::BCC()
{
    INIT_SEGMENTS

    if (s.r.test(CPURegister::StackRegister::C) == false)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        BREAK_SEGMENT(1)

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BCS()
{
    INIT_SEGMENTS

    if (s.r.test(CPURegister::StackRegister::C) == true)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        BREAK_SEGMENT(1)

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BEQ()
{
    INIT_SEGMENTS

    if (s.r.test(CPURegister::StackRegister::Z) == true)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        BREAK_SEGMENT(1)

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BIT()
{
    /* Set Z if result is zero */
    s.r.set(CPURegister::StackRegister::Z, !(s.r.a & s.r.mdr));

    /* Set V to Bit 6 */
    s.r.set(CPURegister::StackRegister::V, s.r.mdr & 0x40u);

    /* Set N to Bit 7 */
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);

    return true;
}

bool CPU::BMI()
{
    INIT_SEGMENTS

    if (s.r.test(CPURegister::StackRegister::N) == true)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        BREAK_SEGMENT(1)

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BNE()
{
    INIT_SEGMENTS

    if (s.r.test(CPURegister::StackRegister::Z) == false)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        BREAK_SEGMENT(1)

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BPL()
{
    INIT_SEGMENTS

    if (s.r.test(CPURegister::StackRegister::N) == false)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        BREAK_SEGMENT(1)

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BRK()
{
    INIT_SEGMENTS

    START_SEGMENT(1)

    /* Set B Flag of SR to true, and push incremented PCH into the stack*/
    bus.cpu_write(s.r.pc >> 8u, 0x100u + s.r.sp--);

    NEXT_SEGMENT(2)

    /* Push PCL into the stack*/
    bus.cpu_write(s.r.pc & 0x00FFu, 0x100u + s.r.sp--);

    NEXT_SEGMENT(3)

    /* Push P into the stack */
    s.r.set(CPURegister::StackRegister::B, true);
    bus.cpu_write(s.r.sr, 0x100u + s.r.sp--);
    s.r.set(CPURegister::StackRegister::B, false);

    NEXT_SEGMENT(4)

    s.r.pc = bus.cpu_read(0xFFFEu);

    END_SEGMENT

    s.r.pc |= bus.cpu_read(0xFFFFu) << 8u;

    FREE_SEGMENTS
    return true;
}

bool CPU::BVC()
{
    INIT_SEGMENTS

    if (s.r.test(CPURegister::StackRegister::V) == false)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        BREAK_SEGMENT(1)

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BVS()
{
    INIT_SEGMENTS

    if (s.r.test(CPURegister::StackRegister::V) == true)
    {
        s.r.pc += static_cast<int8_t>(s.r.mdr);
        BREAK_SEGMENT(1)

        if (s.r.pc >> 8u != s.r.pc - static_cast<int8_t>(s.r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::CLC()
{
    /* Clear Carry Flag */
    s.r.set(CPURegister::StackRegister::C, false);

    return true;
}

bool CPU::CLD()
{
    /* Clear Decimal Flag */
    s.r.set(CPURegister::StackRegister::D, false);

    return true;
}

bool CPU::CLI()
{
    /* Clear Interrupt Disabled Flag */
    s.r.set(CPURegister::StackRegister::I, false);

    return true;
}

bool CPU::CLV()
{
    /* Clear Overflow Flag */
    s.r.set(CPURegister::StackRegister::V, false);

    return true;
}

bool CPU::CMP()
{
    /* Set Flags */
    s.r.set(CPURegister::StackRegister::C, s.r.a >= s.r.mdr);
    s.r.set(CPURegister::StackRegister::Z, s.r.a == s.r.mdr);
    s.r.set(CPURegister::StackRegister::N, (s.r.a - s.r.mdr) & 0x80u);

    return true;
}

bool CPU::CPX()
{
    /* Set Flags */
    s.r.set(CPURegister::StackRegister::C, s.r.x >= s.r.mdr);
    s.r.set(CPURegister::StackRegister::Z, s.r.x == s.r.mdr);
    s.r.set(CPURegister::StackRegister::N, (s.r.x - s.r.mdr) & 0x80u);

    return true;
}

bool CPU::CPY()
{
    /* Set Flags */
    s.r.set(CPURegister::StackRegister::C, s.r.y >= s.r.mdr);
    s.r.set(CPURegister::StackRegister::Z, s.r.y == s.r.mdr);
    s.r.set(CPURegister::StackRegister::N, (s.r.y - s.r.mdr) & 0x80u);

    return true;
}

bool CPU::DEC()
{
    INIT_SEGMENTS

    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        BREAK_SEGMENT(1) /* Re-read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else
        BREAK_SEGMENT(4)
    
    START_SEGMENT(5)

    /* Decrement MDR */
    s.r.mdr -= 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);
    
    END_SEGMENT

    /* Write MDR at address MAR */
    bus.cpu_write(s.r.mdr, s.r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::DEX()
{
    /* Decrement X */
    s.r.x -= 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    return true;
}

bool CPU::DEY()
{
    /* Decrement Y */
    s.r.y -= 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.y == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.y & 0x80u);

    return true;
}

bool CPU::EOR()
{
    /* Set A to A XOR MDR */
    s.r.a ^= s.r.mdr;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    return true;
}

bool CPU::INC()
{
    INIT_SEGMENTS

    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        BREAK_SEGMENT(1) /* Re-read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else
        BREAK_SEGMENT(4)

    START_SEGMENT(5)

    /* Increment MDR */
    s.r.mdr += 1u;

    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);
    
    END_SEGMENT

    bus.cpu_write(s.r.mdr, s.r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::INX()
{
    /* Increment X */
    s.r.x += 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    return true;
}

bool CPU::INY()
{
    /* Increment Y */
    s.r.y += 1u;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.y == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.y & 0x80u);

    return true;
}

bool CPU::JMP()
{
    /* Set PC to MAR */
    s.r.pc = s.r.mar;

    return true;
}

bool CPU::JSR()
{
    INIT_SEGMENTS

    START_SEGMENT(1)
    /* Push Decremented PCH into stack */
    bus.cpu_write(--s.r.pc >> 8u, 0x100u + s.r.sp--);

    NEXT_SEGMENT(2)
    /* Push PCL into stack */
    bus.cpu_write(s.r.pc & 0x00FFu, 0x100u + s.r.sp--);

    END_SEGMENT
    /* Set PC to the Absolute Address */
    s.r.pc = s.r.mar;

    FREE_SEGMENTS
    return true;
}

bool CPU::LDA()
{
    /* Write contents of MDR to A */
    s.r.a = s.r.mdr;
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    return true;
}

bool CPU::LDX()
{
    /* Write contents of MDR to X */
    s.r.x = s.r.mdr;
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    return true;
}

bool CPU::LDY()
{
    /* Write contents of MDR to Y */
    s.r.y = s.r.mdr;
    s.r.set(CPURegister::StackRegister::Z, s.r.y == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.y & 0x80u);

    return true;
}

bool CPU::LSR()
{
    INIT_SEGMENTS

    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        BREAK_SEGMENT(1) /* Re-read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else if (s.mode != CPUMode::ACC)
        BREAK_SEGMENT(4)

    START_SEGMENT(5)

    s.r.set(CPURegister::StackRegister::C, s.r.mdr & 0x01u);
    s.r.mdr >>= 1u;
    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);

    if (s.mode == CPUMode::ACC)
    {
        s.r.a = s.r.mdr;

        FREE_SEGMENTS
        return true;
    }

    END_SEGMENT

    bus.cpu_write(s.r.mdr, s.r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::NOP()
{
    /* Do Nothing */
    return true;
}

bool CPU::ORA()
{
    /* Set A = A | MDR */
    s.r.a |= s.r.mdr; /* Push A into the stack */

    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    return true;
}

bool CPU::PHA()
{
    INIT_SEGMENTS

    START_SEGMENT(1)

    /* Tick after dummy read */

    END_SEGMENT

    /* Push A into the stack */
    bus.cpu_write(s.r.a, 0x100u + s.r.sp--);

    FREE_SEGMENTS
    return true;
}

bool CPU::PHP()
{
    INIT_SEGMENTS

    START_SEGMENT(1)

    /* Tick after dummy read */

    END_SEGMENT

    /* Push P into the stack */
    bus.cpu_write(s.r.sr, 0x100u + s.r.sp--);

    FREE_SEGMENTS
    return true;
}

bool CPU::PLA()
{
    INIT_SEGMENTS

    START_SEGMENT(1)

    /* Tick after dummy read */

    NEXT_SEGMENT(2)

    /* Increment SP */
    ++s.r.sp;
    
    END_SEGMENT;

    /* Pull A from stack */
    s.r.a = bus.cpu_read(0x100u + s.r.sp);

    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    FREE_SEGMENTS
    return true;
}

bool CPU::PLP()
{
    INIT_SEGMENTS

    START_SEGMENT(1)

    /* Tick after dummy read */

    NEXT_SEGMENT(2);

    /* Increment SP */
    ++s.r.sp;

    END_SEGMENT;

    /* Pull P from stack */
    s.r.mdr = s.r.sr;
    s.r.sr = bus.cpu_read(0x100u + s.r.sp);

    s.r.set(CPURegister::StackRegister::X, s.r.mdr & 0x20u);
    s.r.set(CPURegister::StackRegister::B, s.r.mdr & 0x10u);

    FREE_SEGMENTS
    return true;
}

bool CPU::ROL()
{
    INIT_SEGMENTS

    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        BREAK_SEGMENT(1) /* Re-read from effective address */
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else if (s.mode != CPUMode::ACC)
        BREAK_SEGMENT(4)

    START_SEGMENT(5)

    auto C = s.r.mdr >> 7u;

    s.r.mdr = s.r.mdr << 1u | s.r.test(CPURegister::StackRegister::C);

    s.r.set(CPURegister::StackRegister::C, C);
    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr & 0x80u);

    if (s.mode == CPUMode::ACC)
    {
        s.r.a = s.r.mdr;

        FREE_SEGMENTS
        return true;
    }

    END_SEGMENT

    /* Write to effective address */
    bus.cpu_write(s.r.mdr, s.r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::ROR()
{
    INIT_SEGMENTS

    if (s.mode == CPUMode::ABX && s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        BREAK_SEGMENT(1)
    else if (s.mode == CPUMode::ABX && s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else if (s.mode != CPUMode::ACC)
        BREAK_SEGMENT(4)

    START_SEGMENT(5)

    auto C = s.r.mdr & 0x0001u;

    s.r.mdr = s.r.mdr >> 1u | s.r.test(CPURegister::StackRegister::C) << 7u;

    s.r.set(CPURegister::StackRegister::C, C);
    s.r.set(CPURegister::StackRegister::Z, s.r.mdr == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.mdr >> 7u);

    if (s.mode == CPUMode::ACC)
    {
        s.r.a = s.r.mdr;
        
        FREE_SEGMENTS
        return true;
    }

    END_SEGMENT

    /* Write to effective address */
    bus.cpu_write(s.r.mdr, s.r.mar);
    
    FREE_SEGMENTS
    return true;
}

bool CPU::RTI()
{
    INIT_SEGMENTS

    START_SEGMENT(1)

    /* Tick after dummy read */

    NEXT_SEGMENT(2)

    /* Increment Stack Pointer */
    ++s.r.sp;

    NEXT_SEGMENT(3)

    /* Pull P from the Stack */
    s.r.mdr = s.r.sr;
    s.r.sr = bus.cpu_read(0x100u + s.r.sp);

    s.r.set(CPURegister::StackRegister::X, s.r.mdr & 0x20u);
    s.r.set(CPURegister::StackRegister::B, s.r.mdr & 0x10u);

    NEXT_SEGMENT(4)

    /* Pull PCL from the Stack */
    s.r.pc = bus.cpu_read(0x100u + ++s.r.sp);

    END_SEGMENT

    /* Pull PCH from the Stack */
    s.r.pc |= bus.cpu_read(0x100u + ++s.r.sp) << 8u;

    FREE_SEGMENTS
    return true;
}

bool CPU::RTS()
{
    INIT_SEGMENTS

    START_SEGMENT(1)

    /* Tick after dummy read */

    NEXT_SEGMENT(2)

    /* Increment SP */
    s.r.sp++;

    NEXT_SEGMENT(3)

    /* Pull PCL from stack */
    s.r.pc = bus.cpu_read(0x100u + s.r.sp++);

    NEXT_SEGMENT(4)

    /* Pull PCH from stack */
    s.r.pc |= bus.cpu_read(0x100u + s.r.sp) << 8u;

    END_SEGMENT

    /* Increment PC */
    ++s.r.pc;

    FREE_SEGMENTS
    return true;
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

    return true;
}

bool CPU::SEC()
{
    /* Set Carry Flag to True */
    s.r.set(CPURegister::StackRegister::C, true);

    return true;
}

bool CPU::SED()
{
    /* Set Decimal Flag to True */
    s.r.set(CPURegister::StackRegister::D, true);

    return true;
}

bool CPU::SEI()
{
    /* Set Interrupt Disable Flag to True */
    s.r.set(CPURegister::StackRegister::I, true);

    return true;
}

bool CPU::STA()
{
    INIT_SEGMENTS

    /* Take an extra cycle if high byte was not fixed */
    if (s.mode == CPUMode::IDY && (s.r.mar >> 8u == s.r.mar - s.r.y >> 8u))
        BREAK_SEGMENT(1);
    else if (s.mode == CPUMode::ABX && (s.r.mar >> 8u == s.r.mar - s.r.x >> 8u))
        BREAK_SEGMENT(2);
    else if (s.mode == CPUMode::ABY && (s.r.mar >> 8u == s.r.mar - s.r.y >> 8u))
        BREAK_SEGMENT(3);

    /* Store A at MAR */
    bus.cpu_write(s.r.a, s.r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::STX()
{
    /* Store X at MAR */
    bus.cpu_write(s.r.x, s.r.mar);

    return true;
}

bool CPU::STY()
{
    /* Store X at MAR */
    bus.cpu_write(s.r.y, s.r.mar);

    return true;
}

bool CPU::TAX()
{
    /* Transfer A to X */
    s.r.x = s.r.a;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    return true;
}

bool CPU::TAY()
{
    /* Transfer A to Y */
    s.r.y = s.r.a;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.y == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.y & 0x80u);

    return true;
}

bool CPU::TSX()
{
    /* Transfer SP to X */
    s.r.x = s.r.sp;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.x == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.x & 0x80u);

    return true;
}

bool CPU::TXA()
{
    /* Transfer X to A */
    s.r.a = s.r.x;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    return true;
}

bool CPU::TXS()
{
    /* Transfer X to SP */
    s.r.sp = s.r.x;

    return true;
}

bool CPU::TYA()
{
    s.r.a = s.r.y;

    /* Set Flags */
    s.r.set(CPURegister::StackRegister::Z, s.r.a == 0u);
    s.r.set(CPURegister::StackRegister::N, s.r.a & 0x80u);

    return true;
}