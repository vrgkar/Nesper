#include "mode.h"

Task IMP::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /*A dummy read is performed*/
    // bus.cpu_read(s.r.pc);
    
    co_return 0;    
}

Task ACC::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* A dummy read is performed, MDR is assigned to A */
    bus.cpu_read(s.r.pc);
    s.r.mdr = s.r.a;

    co_return 0;
}

Task IMM::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch data from the next byte */
    s.r.mdr = bus.cpu_read(s.r.pc++);

    co_return 0;
}

Task ZPG::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch address into MAR */
    s.r.mar = bus.cpu_read(s.r.pc++);
    s.r.mar &= 0x00FFu;
    co_yield 0;

    /* Fetch data into MDR */
    s.r.mdr = bus.cpu_read(s.r.mar);

    co_return 0;
}

Task ZPX::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Read address into MAR */
    s.r.mar = bus.cpu_read(s.r.pc++);
    co_yield 0;

    /* Add X to MAR */
    s.r.mar += s.r.x;
    s.r.mar &= 0x00FFu;
    co_yield 0;

    /* Read from effective address into MDR*/
    s.r.mdr = bus.cpu_read(s.r.mar);

    co_return 0;
}

Task ZPY::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Read address into MAR */
    s.r.mar = bus.cpu_read(s.r.pc++);
    co_yield 0;

    /* Add Y to MAR */
    s.r.mar += s.r.y;
    s.r.mar &= 0x00FFu;
    co_yield 0;

    /* Read from effective address into MDR*/
    s.r.mdr = bus.cpu_read(s.r.mar & 0x00FFu);

    co_return 0;
}

Task REL::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch offset into MDR */
    s.r.mdr = bus.cpu_read(s.r.pc++);

    co_return 0;
}

Task ABS::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Low byte of address is fetched */
    s.r.mar = bus.cpu_read(s.r.pc++);
    co_yield 0;

    /* High byte of address is fetched */
    s.r.mar |= bus.cpu_read(s.r.pc++) << 8u;

    if (s.opcode == CPUOpcode::JMP)
        co_yield 0;

    /* Read from effective address */
    s.r.mdr = bus.cpu_read(s.r.mar);

    co_return 0;
}

Task ABX::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Low byte of address is fetched */
    s.r.mar = bus.cpu_read(s.r.pc++);
    co_yield 0;

    /* High byte of address is fetched */
    s.r.mar |= bus.cpu_read(s.r.pc++) << 8u;
    s.r.mar += s.r.x;
    co_yield 0;

    /* Fix high address byte */
    if (s.r.mar >> 8u != s.r.mar - s.r.x >> 8u)
        co_yield 1;

    /* Read from effective address */
    s.r.mdr = bus.cpu_read(s.r.mar);

    co_return 0;
}

Task ABY::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Low byte of address is fetched */
    s.r.mar = bus.cpu_read(s.r.pc++);
    co_yield 0;

    /* High byte of address is fetched */
    s.r.mar |= bus.cpu_read(s.r.pc++) << 8u;
    s.r.mar += s.r.y;
    co_yield 0;

    /* Fix high address byte */
    if (s.r.mar >> 8u != s.r.mar - s.r.y >> 8u)
        co_yield 1;

    s.r.mdr = bus.cpu_read(s.r.mar);
    
    co_return 0;
}

Task IND::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Low byte of address is fetched to MAR*/
    s.r.mar = bus.cpu_read(s.r.pc++);
    co_yield 0;

    /* High byte of address is fetched to MAR */
    s.r.mar |= bus.cpu_read(s.r.pc++) << 8u;
    co_yield 0;

    /* Fetch low byte to MDR */
    s.r.mdr = bus.cpu_read(s.r.mar);
    co_yield 0;

    /* Fetch high byte to MAR */
    s.r.mar = bus.cpu_read((s.r.mar & 0xFF00u) | ((s.r.mar + 1u) & 0x00FFu)) << 8u;
    s.r.mar |= s.r.mdr;

    co_return 0;

}

Task IDX::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch pointer address to MAR */
    s.r.mar = bus.cpu_read(s.r.pc++);
    co_yield 0;

    /* Add X to MAR */
    s.r.mar += s.r.x;
    co_yield 0;

    /* Fetch low address byte */
    s.r.mdr = bus.cpu_read(s.r.mar & 0x00FFu);
    co_yield 0;

    /* Fetch high address byte */
    s.r.mar = bus.cpu_read((s.r.mar + 1u) & 0x00FFu) << 8u;
    s.r.mar |= s.r.mdr;
    co_yield 0;

    /* Read from effective address */
    s.r.mdr = bus.cpu_read(s.r.mar);

    co_return 0;
}

Task IDY::execute(Bus &bus, CPUState &s) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch pointer address to MAR */
    s.r.mar = bus.cpu_read(s.r.pc++);
    co_yield 0;

    /* Fetch low address byte from zero page */
    s.r.mdr = bus.cpu_read(s.r.mar & 0x00FFu);
    co_yield 0;

    /* Fetch high address byte from zero page */
    s.r.mar = bus.cpu_read((s.r.mar + 1u) & 0x00FFu) << 8u;
    s.r.mar |= s.r.mdr;

    /* Add Y to low byte of effective address */
    s.r.mar += s.r.y;
    co_yield 0;

    /* *Fix* high byte of address (take extra cycle if page is crossed) */
    if (s.r.mar >> 8u != s.r.mar - s.r.y >> 8u)
        co_yield 1;

    /* Read from effective address */
    s.r.mdr = bus.cpu_read(s.r.mar);
    
    co_return 0;
}