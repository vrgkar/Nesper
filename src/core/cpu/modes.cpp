#include "cpu.h"

bool CPU::IMP()
{
    INIT_SEGMENTS
    

    START_SEGMENT(1)

    /* Tick after fetching opcode */
    
    END_SEGMENT

    /* A dummy read is performed */
    // bus.cpu_read(s.r.pc);
    
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
    // bus.cpu_read(s.r.pc);
    s.r.mdr = s.r.a;

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
    s.r.mdr = bus.cpu_read(s.r.pc++);

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
    s.r.mar = bus.cpu_read(s.r.pc++);
    s.r.mar &= 0x00FFu;

    END_SEGMENT;

    /* Fetch data into MDR */
    s.r.mdr = bus.cpu_read(s.r.mar);

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
    s.r.mar = bus.cpu_read(s.r.pc++);

    NEXT_SEGMENT(3)

    /* Add X to MAR */
    s.r.mar += s.r.x;
    s.r.mar &= 0x00FFu;

    END_SEGMENT

    /* Read from effective address into MDR */
    s.r.mdr = bus.cpu_read(s.r.mar);

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
    s.r.mar = bus.cpu_read(s.r.pc++);

    NEXT_SEGMENT(3)

    /* Add Y to MAR */
    s.r.mar += s.r.y;
    s.r.mar &= 0x00FFu;

    END_SEGMENT

    /* Read from effective address into MDR */
    s.r.mdr = bus.cpu_read(s.r.mar & 0x00FFu);

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
    s.r.mdr = bus.cpu_read(s.r.pc++);

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
    s.r.mar = bus.cpu_read(s.r.pc++);
    
    NEXT_SEGMENT(3)

    /* High byte of address is fetched */
    s.r.mar |= bus.cpu_read(s.r.pc++) << 8u;

    if (s.opcode != CPUOpcode::JMP)
    {
        /* Read from effective address */
        s.r.mdr = bus.cpu_read(s.r.mar);

        FREE_SEGMENTS
        return true;
    }
    
    END_SEGMENT

    /* Read from effective address */
    s.r.mdr = bus.cpu_read(s.r.mar);

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
    s.r.mar = bus.cpu_read(s.r.pc++);

    NEXT_SEGMENT(3);

    /* High byte of address is fetched */
    s.r.mar |= bus.cpu_read(s.r.pc++) << 8u;
    s.r.mar += s.r.x;
    
    NEXT_SEGMENT(4)

    /* If we don't need to fix the high address byte, read and return */
    if (s.r.mar >> 8u == s.r.mar - s.r.x >> 8u)
    {
        /* Read from effective address */
        s.r.mdr = bus.cpu_read(s.r.mar);

        FREE_SEGMENTS
        return true;
    }
    
    END_SEGMENT;

    /* Read from effective address */
    s.r.mdr = bus.cpu_read(s.r.mar);

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
    s.r.mar = bus.cpu_read(s.r.pc++);
    
    NEXT_SEGMENT(3)

    /* High byte of address is fetched */
    s.r.mar |= bus.cpu_read(s.r.pc++) << 8u;
    s.r.mar += s.r.y;
    
    NEXT_SEGMENT(4)

    /* If we don't need to fix the high address byte, read and return */
    if (s.r.mar >> 8u == s.r.mar - s.r.y >> 8u)
    {
        s.r.mdr = bus.cpu_read(s.r.mar);

        FREE_SEGMENTS
        return true;
    }
    
    END_SEGMENT

    s.r.mdr = bus.cpu_read(s.r.mar);
    
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
    s.r.mar = bus.cpu_read(s.r.pc++);

    NEXT_SEGMENT(3)

    /* High byte of address is fetched to MAR */
    s.r.mar |= bus.cpu_read(s.r.pc++) << 8u;

    NEXT_SEGMENT(4)
    
    /* Fetch low byte to MDR */
    s.r.mdr = bus.cpu_read(s.r.mar);

    END_SEGMENT

    /* Fetch high byte to MAR */
    s.r.mar = bus.cpu_read((s.r.mar & 0xFF00u) | ((s.r.mar + 1u) & 0x00FFu)) << 8u;
    s.r.mar |= s.r.mdr;

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
    s.r.mar = bus.cpu_read(s.r.pc++);

    NEXT_SEGMENT(3)

    /* Add X to MAR */
    s.r.mar += s.r.x;

    NEXT_SEGMENT(4)
    
    /* Fetch low address byte */
    s.r.mdr = bus.cpu_read(s.r.mar & 0x00FFu);
    
    NEXT_SEGMENT(5)

    /* Fetch high address byte */
    s.r.mar = bus.cpu_read((s.r.mar + 1u) & 0x00FFu) << 8u;
    s.r.mar |= s.r.mdr;
    
    END_SEGMENT

    /* Read from effective address */
    s.r.mdr = bus.cpu_read(s.r.mar);

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
    s.r.mar = bus.cpu_read(s.r.pc++);

    NEXT_SEGMENT(3)

    /* Fetch low address byte from zero page */
    s.r.mdr = bus.cpu_read(s.r.mar & 0x00FFu);

    NEXT_SEGMENT(4)
    /* Fetch high address byte from zero page */
    s.r.mar = bus.cpu_read((s.r.mar + 1u) & 0x00FFu) << 8u;
    s.r.mar |= s.r.mdr;

    /* Add Y to low byte of effective address */
    s.r.mar += s.r.y;

    NEXT_SEGMENT(5)

    /* If high byte of address doesn't need to be fixed, read and return */
    if (s.r.mar >> 8u == s.r.mar - s.r.y >> 8u)
    {
        /* Read from effective address */
        s.r.mdr = bus.cpu_read(s.r.mar);
        
        FREE_SEGMENTS
        return true;
    }
    
    END_SEGMENT

    /* Read from effective address */
    s.r.mdr = bus.cpu_read(s.r.mar);
    
    FREE_SEGMENTS
    return true;
}