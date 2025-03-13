#include "cpu.h"

bool CPU::RST()
{
    INIT_SEGMENTS

    START_SEGMENT(1)
    NEXT_SEGMENT(2)
    NEXT_SEGMENT(3)
    NEXT_SEGMENT(4)
    NEXT_SEGMENT(5)

    /* SP gets decremented thrice */
    m_r.sp -= 3u;

    NEXT_SEGMENT(6)
    
    m_r.pc = m_bus->read_cpu(0xFFFCu);

    END_SEGMENT

    m_r.pc |= m_bus->read_cpu(0xFFFDu) << 8u;

    FREE_SEGMENTS
    return true;
}

bool CPU::IRQ()
{
    return true;
}

bool CPU::NMI()
{
    INIT_SEGMENTS

    START_SEGMENT(1)
    // Force BRK into the opcode register
    m_opcode = 0x00u;

    NEXT_SEGMENT(2)

    // Fetch another byte which is discarded

    NEXT_SEGMENT(3)

    // Push PCH onto the stack
    m_bus->write_cpu(m_r.pc >> 8u, 0x0100u + m_r.sp--);

    NEXT_SEGMENT(4)

    // Push PCL onto the stack
    m_bus->write_cpu(m_r.pc & 0x00FFu, 0x0100u + m_r.sp--);
    
    NEXT_SEGMENT(5)

    // Push status register onto the stack with the B flag cleared
    m_r.set(Register::Status::B, false);
    m_bus->write_cpu(m_r.sr, 0x0100u + m_r.sp--);
    
    NEXT_SEGMENT(6)
    
    // Fetch PCL
    m_r.set(CPU::Register::Status::I, true);
    m_r.pc = m_bus->read_cpu(0xFFFAu);

    END_SEGMENT

    m_r.pc |= m_bus->read_cpu(0xFFFBu) << 8u;

    FREE_SEGMENTS
    return true;
}