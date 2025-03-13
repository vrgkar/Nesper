#include "cpu.h"

bool CPU::ADC()
{
    uint16_t temp = m_r.a + m_r.mdr + m_r.test(Register::Status::C);

    m_r.set(Register::Status::C, temp > 0x00FFu);
    m_r.set(Register::Status::Z, (temp & 0x00FFu) == 0u);
    m_r.set(Register::Status::V, ((temp ^ m_r.a) & ~(m_r.a ^ m_r.mdr)) & 0x0080u);
    m_r.set(Register::Status::N, temp & 0x0080u);

    /* Wrte sum into A */
    m_r.a = temp & 0x00FFu;

    return true;
}

bool CPU::AND()
{
    /* Perform AND with A and MDR and write to A */
    m_r.a &= m_r.mdr;
    m_r.set(Register::Status::Z, m_r.a == 0x00u);
    m_r.set(Register::Status::N, m_r.a & 0x80u);

    return true;
}

bool CPU::ASL()
{
    INIT_SEGMENTS

    if (m_mode == Mode::ABX && m_r.mar >> 8u != m_r.mar - m_r.x >> 8u)
    {
        BREAK_SEGMENT(1) /* Re read from effective address */
    }
    else if (m_mode == Mode::ABX && m_r.mar >> 8u == m_r.mar - m_r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else if (m_mode != Mode::ACC)
    {
        BREAK_SEGMENT(4)
    }

    START_SEGMENT(5)

    if (m_mode != Mode::ACC)
        m_bus->write_cpu(m_r.mdr, m_r.mar);

    m_r.set(Register::Status::C, m_r.mdr & 0x80u);
    m_r.mdr <<= 1u;
    m_r.set(Register::Status::Z, m_r.mdr == 0u);
    m_r.set(Register::Status::N, m_r.mdr & 0x80u);

    if (m_mode == Mode::ACC)
    {
        /* A is set to MDR */
        m_r.a = m_r.mdr;

        FREE_SEGMENTS
        return true;
    }

    END_SEGMENT

    m_bus->write_cpu(m_r.mdr, m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::BCC()
{
    INIT_SEGMENTS

    if (m_r.test(Register::Status::C) == false)
    {
        START_SEGMENT(1)
        m_r.pc += static_cast<int8_t>(m_r.mdr);
        END_SEGMENT

        if (m_r.pc >> 8u != m_r.pc - static_cast<int8_t>(m_r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BCS()
{
    INIT_SEGMENTS

    if (m_r.test(Register::Status::C) == true)
    {
        START_SEGMENT(1)
        m_r.pc += static_cast<int8_t>(m_r.mdr);
        END_SEGMENT

        if (m_r.pc >> 8u != m_r.pc - static_cast<int8_t>(m_r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BEQ()
{
    INIT_SEGMENTS

    if (m_r.test(Register::Status::Z) == true)
    {
        START_SEGMENT(1)
        m_r.pc += static_cast<int8_t>(m_r.mdr);
        END_SEGMENT

        if (m_r.pc >> 8u != m_r.pc - static_cast<int8_t>(m_r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BIT()
{
    /* Set Z if result is zero */
    m_r.set(Register::Status::Z, !(m_r.a & m_r.mdr));

    /* Set V to Bit 6 */
    m_r.set(Register::Status::V, m_r.mdr & 0x40u);

    /* Set N to Bit 7 */
    m_r.set(Register::Status::N, m_r.mdr & 0x80u);

    return true;
}

bool CPU::BMI()
{
    INIT_SEGMENTS

    if (m_r.test(Register::Status::N) == true)
    {
        START_SEGMENT(1)
        m_r.pc += static_cast<int8_t>(m_r.mdr);
        END_SEGMENT

        if (m_r.pc >> 8u != m_r.pc - static_cast<int8_t>(m_r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BNE()
{
    INIT_SEGMENTS

    if (m_r.test(Register::Status::Z) == false)
    {
        START_SEGMENT(1)
        m_r.pc += static_cast<int8_t>(m_r.mdr);
        END_SEGMENT

        if (m_r.pc >> 8u != m_r.pc - static_cast<int8_t>(m_r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BPL()
{
    INIT_SEGMENTS

    if (m_r.test(Register::Status::N) == false)
    {
        START_SEGMENT(1)
        m_r.pc += static_cast<int8_t>(m_r.mdr);
        END_SEGMENT

        if (m_r.pc >> 8u != m_r.pc - static_cast<int8_t>(m_r.mdr) >> 8u)
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
    m_bus->write_cpu(m_r.pc >> 8u, 0x100u + m_r.sp--);

    NEXT_SEGMENT(2)

    /* Push PCL into the stack*/
    m_bus->write_cpu(m_r.pc & 0x00FFu, 0x100u + m_r.sp--);

    NEXT_SEGMENT(3)

    /* Push P into the stack */
    m_r.set(Register::Status::B, true);
    m_bus->write_cpu(m_r.sr, 0x100u + m_r.sp--);
    m_r.set(Register::Status::B, false);

    NEXT_SEGMENT(4)

    m_r.pc = m_bus->read_cpu(0xFFFEu);

    END_SEGMENT

    m_r.pc |= m_bus->read_cpu(0xFFFFu) << 8u;

    FREE_SEGMENTS
    return true;
}

bool CPU::BVC()
{
    INIT_SEGMENTS

    if (m_r.test(Register::Status::V) == false)
    {
        START_SEGMENT(1)
        m_r.pc += static_cast<int8_t>(m_r.mdr);
        END_SEGMENT

        if (m_r.pc >> 8u != m_r.pc - static_cast<int8_t>(m_r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::BVS()
{
    INIT_SEGMENTS

    if (m_r.test(Register::Status::V) == true)
    {
        START_SEGMENT(1)
        m_r.pc += static_cast<int8_t>(m_r.mdr);
        END_SEGMENT

        if (m_r.pc >> 8u != m_r.pc - static_cast<int8_t>(m_r.mdr) >> 8u)
            BREAK_SEGMENT(2)
    }

    FREE_SEGMENTS
    return true;
}

bool CPU::CLC()
{
    /* Clear Carry Flag */
    m_r.set(Register::Status::C, false);

    return true;
}

bool CPU::CLD()
{
    /* Clear Decimal Flag */
    m_r.set(Register::Status::D, false);

    return true;
}

bool CPU::CLI()
{
    /* Clear Interrupt Disabled Flag */
    m_r.set(Register::Status::I, false);

    return true;
}

bool CPU::CLV()
{
    /* Clear Overflow Flag */
    m_r.set(Register::Status::V, false);

    return true;
}

bool CPU::CMP()
{
    /* Set Flags */
    m_r.set(Register::Status::C, m_r.a >= m_r.mdr);
    m_r.set(Register::Status::Z, m_r.a == m_r.mdr);
    m_r.set(Register::Status::N, (m_r.a - m_r.mdr) & 0x80u);

    return true;
}

bool CPU::CPX()
{
    /* Set Flags */
    m_r.set(Register::Status::C, m_r.x >= m_r.mdr);
    m_r.set(Register::Status::Z, m_r.x == m_r.mdr);
    m_r.set(Register::Status::N, (m_r.x - m_r.mdr) & 0x80u);

    return true;
}

bool CPU::CPY()
{
    /* Set Flags */
    m_r.set(Register::Status::C, m_r.y >= m_r.mdr);
    m_r.set(Register::Status::Z, m_r.y == m_r.mdr);
    m_r.set(Register::Status::N, (m_r.y - m_r.mdr) & 0x80u);

    return true;
}

bool CPU::DEC()
{
    INIT_SEGMENTS

    if (m_mode == Mode::ABX && m_r.mar >> 8u != m_r.mar - m_r.x >> 8u)
    {
        BREAK_SEGMENT(1) /* Re-read from effective address */
    }
    else if (m_mode == Mode::ABX && m_r.mar >> 8u == m_r.mar - m_r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else
    {
        BREAK_SEGMENT(4)
    }
    
    START_SEGMENT(5)

    /* Decrement MDR */
    m_r.mdr -= 1u;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.mdr == 0u);
    m_r.set(Register::Status::N, m_r.mdr & 0x80u);
    
    END_SEGMENT

    /* Write MDR at address MAR */
    m_bus->write_cpu(m_r.mdr, m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::DEX()
{
    /* Decrement X */
    m_r.x -= 1u;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.x == 0u);
    m_r.set(Register::Status::N, m_r.x & 0x80u);

    return true;
}

bool CPU::DEY()
{
    /* Decrement Y */
    m_r.y -= 1u;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.y == 0u);
    m_r.set(Register::Status::N, m_r.y & 0x80u);

    return true;
}

bool CPU::EOR()
{
    /* Set A to A XOR MDR */
    m_r.a ^= m_r.mdr;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.a == 0u);
    m_r.set(Register::Status::N, m_r.a & 0x80u);

    return true;
}

bool CPU::INC()
{
    INIT_SEGMENTS

    if (m_mode == Mode::ABX && m_r.mar >> 8u != m_r.mar - m_r.x >> 8u)
    {
        BREAK_SEGMENT(1) /* Re-read from effective address */
    }
    else if (m_mode == Mode::ABX && m_r.mar >> 8u == m_r.mar - m_r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else
    {
        BREAK_SEGMENT(4)
    }

    START_SEGMENT(5)

    /* Increment MDR */
    m_r.mdr += 1u;

    m_r.set(Register::Status::Z, m_r.mdr == 0u);
    m_r.set(Register::Status::N, m_r.mdr & 0x80u);
    
    END_SEGMENT

    m_bus->write_cpu(m_r.mdr, m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::INX()
{
    /* Increment X */
    m_r.x += 1u;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.x == 0u);
    m_r.set(Register::Status::N, m_r.x & 0x80u);

    return true;
}

bool CPU::INY()
{
    /* Increment Y */
    m_r.y += 1u;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.y == 0u);
    m_r.set(Register::Status::N, m_r.y & 0x80u);

    return true;
}

bool CPU::JMP()
{
    /* Set PC to MAR */
    m_r.pc = m_r.mar;

    return true;
}

bool CPU::JSR()
{
    INIT_SEGMENTS


    START_SEGMENT(1)
    /* Push Decremented PCH into stack */
    m_bus->write_cpu(--m_r.pc >> 8u, 0x100u + m_r.sp--);

    NEXT_SEGMENT(2)
    /* Push PCL into stack */
    m_bus->write_cpu(m_r.pc & 0x00FFu, 0x100u + m_r.sp--);

    END_SEGMENT
    /* Set PC to the Absolute Address */
    m_r.pc = m_r.mar;

    FREE_SEGMENTS
    return true;
}

bool CPU::LDA()
{
    /* Write contents of MDR to A */
    m_r.a = m_r.mdr;
    m_r.set(Register::Status::Z, m_r.a == 0u);
    m_r.set(Register::Status::N, m_r.a & 0x80u);

    return true;
}

bool CPU::LDX()
{
    /* Write contents of MDR to X */
    m_r.x = m_r.mdr;
    m_r.set(Register::Status::Z, m_r.x == 0u);
    m_r.set(Register::Status::N, m_r.x & 0x80u);

    return true;
}

bool CPU::LDY()
{
    /* Write contents of MDR to Y */
    m_r.y = m_r.mdr;
    m_r.set(Register::Status::Z, m_r.y == 0u);
    m_r.set(Register::Status::N, m_r.y & 0x80u);

    return true;
}

bool CPU::LSR()
{
    INIT_SEGMENTS

    if (m_mode == Mode::ABX && m_r.mar >> 8u != m_r.mar - m_r.x >> 8u)
    {
        BREAK_SEGMENT(1) /* Re-read from effective address */
    }
    else if (m_mode == Mode::ABX && m_r.mar >> 8u == m_r.mar - m_r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else if (m_mode != Mode::ACC)
    {
        BREAK_SEGMENT(4)
    }

    START_SEGMENT(5)

    m_r.set(Register::Status::C, m_r.mdr & 0x01u);
    m_r.mdr >>= 1u;
    m_r.set(Register::Status::Z, m_r.mdr == 0u);
    m_r.set(Register::Status::N, m_r.mdr & 0x80u);

    if (m_mode == Mode::ACC)
    {
        m_r.a = m_r.mdr;

        FREE_SEGMENTS
        return true;
    }

    END_SEGMENT

    m_bus->write_cpu(m_r.mdr, m_r.mar);

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
    m_r.a |= m_r.mdr; /* Push A into the stack */

    m_r.set(Register::Status::Z, m_r.a == 0u);
    m_r.set(Register::Status::N, m_r.a & 0x80u);

    return true;
}

bool CPU::PHA()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after dummy read */

    END_SEGMENT

    /* Push A into the stack */
    m_bus->write_cpu(m_r.a, 0x100u + m_r.sp--);

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
    m_bus->write_cpu(m_r.sr, 0x100u + m_r.sp--);

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
    ++m_r.sp;
    
    END_SEGMENT;

    /* Pull A from stack */
    m_r.a = m_bus->read_cpu(0x100u + m_r.sp);

    m_r.set(Register::Status::Z, m_r.a == 0u);
    m_r.set(Register::Status::N, m_r.a & 0x80u);

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
    ++m_r.sp;

    END_SEGMENT;

    /* Pull P from stack */
    m_r.mdr = m_r.sr;
    m_r.sr = m_bus->read_cpu(0x100u + m_r.sp);

    m_r.set(Register::Status::X, m_r.mdr & 0x20u);
    m_r.set(Register::Status::B, m_r.mdr & 0x10u);

    FREE_SEGMENTS
    return true;
}

bool CPU::ROL()
{
    INIT_SEGMENTS

    if (m_mode == Mode::ABX && m_r.mar >> 8u != m_r.mar - m_r.x >> 8u)
    {
        BREAK_SEGMENT(1) /* Re-read from effective address */
    }
    else if (m_mode == Mode::ABX && m_r.mar >> 8u == m_r.mar - m_r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else if (m_mode != Mode::ACC)
    {
        BREAK_SEGMENT(4)
    }

    START_SEGMENT(5)

    auto C = m_r.mdr >> 7u;

    m_r.mdr = m_r.mdr << 1u | m_r.test(Register::Status::C);

    m_r.set(Register::Status::C, C);
    m_r.set(Register::Status::Z, m_r.mdr == 0u);
    m_r.set(Register::Status::N, m_r.mdr & 0x80u);

    if (m_mode == Mode::ACC)
    {
        m_r.a = m_r.mdr;

        FREE_SEGMENTS
        return true;
    }

    END_SEGMENT

    /* Write to effective address */
    m_bus->write_cpu(m_r.mdr, m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::ROR()
{
    INIT_SEGMENTS

    if (m_mode == Mode::ABX && m_r.mar >> 8u != m_r.mar - m_r.x >> 8u)
    {
        BREAK_SEGMENT(1)
    }
    else if (m_mode == Mode::ABX && m_r.mar >> 8u == m_r.mar - m_r.x >> 8u)
    {
        /* Fix high address byte */
        BREAK_SEGMENT(2)

        /* Re-read from effective address */
        BREAK_SEGMENT(3)
    }
    else if (m_mode != Mode::ACC)
    {
        BREAK_SEGMENT(4)
    }

    START_SEGMENT(5)

    auto C = m_r.mdr & 0x0001u;

    m_r.mdr = m_r.mdr >> 1u | m_r.test(Register::Status::C) << 7u;

    m_r.set(Register::Status::C, C);
    m_r.set(Register::Status::Z, m_r.mdr == 0u);
    m_r.set(Register::Status::N, m_r.mdr >> 7u);

    if (m_mode == Mode::ACC)
    {
        m_r.a = m_r.mdr;
        
        FREE_SEGMENTS
        return true;
    }

    END_SEGMENT

    /* Write to effective address */
    m_bus->write_cpu(m_r.mdr, m_r.mar);
    
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
    ++m_r.sp;

    NEXT_SEGMENT(3)

    /* Pull P from the Stack */
    m_r.mdr = m_r.sr;
    m_r.sr = m_bus->read_cpu(0x100u + m_r.sp);

    m_r.set(Register::Status::X, m_r.mdr & 0x20u);
    m_r.set(Register::Status::B, m_r.mdr & 0x10u);

    NEXT_SEGMENT(4)

    /* Pull PCL from the Stack */
    m_r.pc = m_bus->read_cpu(0x100u + ++m_r.sp);

    END_SEGMENT

    /* Pull PCH from the Stack */
    m_r.pc |= m_bus->read_cpu(0x100u + ++m_r.sp) << 8u;

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
    m_r.sp++;

    NEXT_SEGMENT(3)

    /* Pull PCL from stack */
    m_r.pc = m_bus->read_cpu(0x100u + m_r.sp++);

    NEXT_SEGMENT(4)

    /* Pull PCH from stack */
    m_r.pc |= m_bus->read_cpu(0x100u + m_r.sp) << 8u;

    END_SEGMENT

    /* Increment PC */
    ++m_r.pc;

    FREE_SEGMENTS
    return true;
}

bool CPU::SBC()
{
    /* Invert bits of MDR */
    uint16_t value = m_r.mdr ^ 0x00FFu;

    uint16_t temp = m_r.a + value + m_r.test(Register::Status::C);

    m_r.set(Register::Status::C, temp > 0x00FFu);
    m_r.set(Register::Status::Z, (temp & 0x00FFu) == 0u);
    m_r.set(Register::Status::V, ((temp ^ m_r.a) & (m_r.a ^ m_r.mdr)) & 0x0080u);
    m_r.set(Register::Status::N, temp & 0x0080u);

    m_r.a = temp & 0x00FFu;

    return true;
}

bool CPU::SEC()
{
    /* Set Carry Flag to True */
    m_r.set(Register::Status::C, true);

    return true;
}

bool CPU::SED()
{
    /* Set Decimal Flag to True */
    m_r.set(Register::Status::D, true);

    return true;
}

bool CPU::SEI()
{
    /* Set Interrupt Disable Flag to True */
    m_r.set(Register::Status::I, true);

    return true;
}

bool CPU::STA()
{
    INIT_SEGMENTS

    /* Take an extra cycle if high byte was not fixed */
    if (m_mode == Mode::IDY && (m_r.mar >> 8u == m_r.mar - m_r.y >> 8u))
    {
        BREAK_SEGMENT(1)
    }
    else if (m_mode == Mode::ABX && (m_r.mar >> 8u == m_r.mar - m_r.x >> 8u))
    {
        BREAK_SEGMENT(2)
    }
    else if (m_mode == Mode::ABY && (m_r.mar >> 8u == m_r.mar - m_r.y >> 8u))
    {
        BREAK_SEGMENT(3)
    }

    /* Store A at MAR */
    m_bus->write_cpu(m_r.a, m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::STX()
{
    /* Store X at MAR */
    m_bus->write_cpu(m_r.x, m_r.mar);

    return true;
}

bool CPU::STY()
{
    /* Store X at MAR */
    m_bus->write_cpu(m_r.y, m_r.mar);

    return true;
}

bool CPU::TAX()
{
    /* Transfer A to X */
    m_r.x = m_r.a;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.x == 0u);
    m_r.set(Register::Status::N, m_r.x & 0x80u);

    return true;
}

bool CPU::TAY()
{
    /* Transfer A to Y */
    m_r.y = m_r.a;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.y == 0u);
    m_r.set(Register::Status::N, m_r.y & 0x80u);

    return true;
}

bool CPU::TSX()
{
    /* Transfer SP to X */
    m_r.x = m_r.sp;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.x == 0u);
    m_r.set(Register::Status::N, m_r.x & 0x80u);

    return true;
}

bool CPU::TXA()
{
    /* Transfer X to A */
    m_r.a = m_r.x;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.a == 0u);
    m_r.set(Register::Status::N, m_r.a & 0x80u);

    return true;
}

bool CPU::TXS()
{
    /* Transfer X to SP */
    m_r.sp = m_r.x;

    return true;
}

bool CPU::TYA()
{
    m_r.a = m_r.y;

    /* Set Flags */
    m_r.set(Register::Status::Z, m_r.a == 0u);
    m_r.set(Register::Status::N, m_r.a & 0x80u);

    return true;
}