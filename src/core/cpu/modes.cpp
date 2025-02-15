#include "cpu.h"

bool CPU::IMP()
{
    INIT_SEGMENTS
    

    START_SEGMENT(1)

    /* Tick after fetching opcode */
    
    END_SEGMENT

    /* A dummy read is performed */
    // m_bus->cpu_read(m_r.pc);
    
    FREE_SEGMENTS
    return true;    
}

bool CPU::ACC()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */
    
    END_SEGMENT

    /* A dummy read is performed, MDR is assigned to A */
    // m_bus->cpu_read(m_r.pc);
    m_r.mdr = m_r.a;

    FREE_SEGMENTS
    return true;
}

bool CPU::IMM()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */
    
    END_SEGMENT

    /* Fetch data from the next byte */
    m_r.mdr = m_bus->cpu_read(m_r.pc++);

    FREE_SEGMENTS
    return true;
}

bool CPU::ZPG()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */

    NEXT_SEGMENT(2);

    /* Fetch address into MAR */
    m_r.mar = m_bus->cpu_read(m_r.pc++);
    m_r.mar &= 0x00FFu;

    END_SEGMENT;

    /* Fetch data into MDR */
    m_r.mdr = m_bus->cpu_read(m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::ZPX()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */

    NEXT_SEGMENT(2)

    /* Read address into MAR */
    m_r.mar = m_bus->cpu_read(m_r.pc++);

    NEXT_SEGMENT(3)

    /* Add X to MAR */
    m_r.mar += m_r.x;
    m_r.mar &= 0x00FFu;

    END_SEGMENT

    /* Read from effective address into MDR */
    m_r.mdr = m_bus->cpu_read(m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::ZPY()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */

    NEXT_SEGMENT(2)
    
    /* Read address into MAR */
    m_r.mar = m_bus->cpu_read(m_r.pc++);

    NEXT_SEGMENT(3)

    /* Add Y to MAR */
    m_r.mar += m_r.y;
    m_r.mar &= 0x00FFu;

    END_SEGMENT

    /* Read from effective address into MDR */
    m_r.mdr = m_bus->cpu_read(m_r.mar & 0x00FFu);

    FREE_SEGMENTS
    return true;
}

bool CPU::REL()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */

    END_SEGMENT;

    /* Fetch offset into MDR */
    m_r.mdr = m_bus->cpu_read(m_r.pc++);

    FREE_SEGMENTS
    return true;
}

bool CPU::ABS()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */
    
    NEXT_SEGMENT(2)

    /* Low byte of address is fetched */
    m_r.mar = m_bus->cpu_read(m_r.pc++);
    
    NEXT_SEGMENT(3)

    /* High byte of address is fetched */
    m_r.mar |= m_bus->cpu_read(m_r.pc++) << 8u;

    if (m_instr != Instruction::JMP)
    {
        /* Read from effective address */
        m_r.mdr = m_bus->cpu_read(m_r.mar);

        FREE_SEGMENTS
        return true;
    }
    
    END_SEGMENT

    /* Read from effective address */
    m_r.mdr = m_bus->cpu_read(m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::ABX()
{
    INIT_SEGMENTS

    
    START_SEGMENT(1)

    /* Tick after fetching opcode */

    NEXT_SEGMENT(2);

    /* Low byte of address is fetched */
    m_r.mar = m_bus->cpu_read(m_r.pc++);

    NEXT_SEGMENT(3);

    /* High byte of address is fetched */
    m_r.mar |= m_bus->cpu_read(m_r.pc++) << 8u;
    m_r.mar += m_r.x;
    
    NEXT_SEGMENT(4)

    /* If we don't need to fix the high address byte, read and return */
    if (m_r.mar >> 8u == m_r.mar - m_r.x >> 8u)
    {
        /* Read from effective address */
        m_r.mdr = m_bus->cpu_read(m_r.mar);

        FREE_SEGMENTS
        return true;
    }
    
    END_SEGMENT;

    /* Read from effective address */
    m_r.mdr = m_bus->cpu_read(m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::ABY()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */
    
    NEXT_SEGMENT(2)

    /* Low byte of address is fetched */
    m_r.mar = m_bus->cpu_read(m_r.pc++);
    
    NEXT_SEGMENT(3)

    /* High byte of address is fetched */
    m_r.mar |= m_bus->cpu_read(m_r.pc++) << 8u;
    m_r.mar += m_r.y;
    
    NEXT_SEGMENT(4)

    /* If we don't need to fix the high address byte, read and return */
    if (m_r.mar >> 8u == m_r.mar - m_r.y >> 8u)
    {
        m_r.mdr = m_bus->cpu_read(m_r.mar);

        FREE_SEGMENTS
        return true;
    }
    
    END_SEGMENT

    m_r.mdr = m_bus->cpu_read(m_r.mar);
    
    FREE_SEGMENTS
    return true;
}

bool CPU::IND()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */

    NEXT_SEGMENT(2)

    /* Low byte of address is fetched to MAR*/
    m_r.mar = m_bus->cpu_read(m_r.pc++);

    NEXT_SEGMENT(3)

    /* High byte of address is fetched to MAR */
    m_r.mar |= m_bus->cpu_read(m_r.pc++) << 8u;

    NEXT_SEGMENT(4)
    
    /* Fetch low byte to MDR */
    m_r.mdr = m_bus->cpu_read(m_r.mar);

    END_SEGMENT

    /* Fetch high byte to MAR */
    m_r.mar = m_bus->cpu_read((m_r.mar & 0xFF00u) | ((m_r.mar + 1u) & 0x00FFu)) << 8u;
    m_r.mar |= m_r.mdr;

    FREE_SEGMENTS
    return true;

}

bool CPU::IDX()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */

    NEXT_SEGMENT(2)

    /* Fetch pointer address to MAR */
    m_r.mar = m_bus->cpu_read(m_r.pc++);

    NEXT_SEGMENT(3)

    /* Add X to MAR */
    m_r.mar += m_r.x;

    NEXT_SEGMENT(4)
    
    /* Fetch low address byte */
    m_r.mdr = m_bus->cpu_read(m_r.mar & 0x00FFu);
    
    NEXT_SEGMENT(5)

    /* Fetch high address byte */
    m_r.mar = m_bus->cpu_read((m_r.mar + 1u) & 0x00FFu) << 8u;
    m_r.mar |= m_r.mdr;
    
    END_SEGMENT

    /* Read from effective address */
    m_r.mdr = m_bus->cpu_read(m_r.mar);

    FREE_SEGMENTS
    return true;
}

bool CPU::IDY()
{
    INIT_SEGMENTS


    START_SEGMENT(1)

    /* Tick after fetching opcode */

    NEXT_SEGMENT(2)

    /* Fetch pointer address to MAR */
    m_r.mar = m_bus->cpu_read(m_r.pc++);

    NEXT_SEGMENT(3)

    /* Fetch low address byte from zero page */
    m_r.mdr = m_bus->cpu_read(m_r.mar & 0x00FFu);

    NEXT_SEGMENT(4)
    /* Fetch high address byte from zero page */
    m_r.mar = m_bus->cpu_read((m_r.mar + 1u) & 0x00FFu) << 8u;
    m_r.mar |= m_r.mdr;

    /* Add Y to low byte of effective address */
    m_r.mar += m_r.y;

    NEXT_SEGMENT(5)

    /* If high byte of address doesn't need to be fixed, read and return */
    if (m_r.mar >> 8u == m_r.mar - m_r.y >> 8u)
    {
        /* Read from effective address */
        m_r.mdr = m_bus->cpu_read(m_r.mar);
        
        FREE_SEGMENTS
        return true;
    }
    
    END_SEGMENT

    /* Read from effective address */
    m_r.mdr = m_bus->cpu_read(m_r.mar);
    
    FREE_SEGMENTS
    return true;
}