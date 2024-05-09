#include "mode.h"
#include "../cpu.h"

ModeTask IMP::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /*A dummy read is performed*/
    cpu.read(r.pc);
    
    co_return 0;    
}

ModeTask ACC::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* A dummy read is performed, MDR is assigned to A */
    cpu.read(r.pc);
    r.mdr = r.a;

    co_return 0;
}

ModeTask IMM::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch data from the next byte */
    r.mdr = cpu.read(r.pc++);

    co_return 0;
}

ModeTask ZPG::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch address into MAR */
    r.mar = cpu.read(r.pc++);
    r.mar &= 0x00FFu;
    co_yield 0;

    /* Fetch data into MDR */
    r.mdr = cpu.read(r.mar);

    co_return 0;
}

ModeTask ZPX::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Read address into MAR */
    r.mar = cpu.read(r.pc++);
    co_yield 0;

    /* Add X to MAR */
    r.mar += r.x;
    r.mar &= 0x00FFu;
    co_yield 0;

    /* Read from effective address into MDR*/
    r.mdr = cpu.read(r.mar);

    co_return 0;
}

ModeTask ZPY::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Read address into MAR */
    r.mar = cpu.read(r.pc++);
    co_yield 0;

    /* Add Y to MAR */
    r.mar += r.y;
    r.mar &= 0x00FFu;
    co_yield 0;

    /* Read from effective address into MDR*/
    r.mdr = cpu.read(r.mar & 0x00FFu);

    co_return 0;
}

ModeTask REL::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch offset into MDR */
    r.mdr = cpu.read(r.pc++);

    co_return 0;
}

ModeTask ABS::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Low byte of address is fetched */
    r.mar = cpu.read(r.pc++);
    co_yield 0;

    /* High byte of address is fetched */
    r.mar |= cpu.read(r.pc++) << 8u;

    if (cpu.get_instruction_id() != "JMP")
        co_yield 0;

    /* Read from effective address */
    r.mdr = cpu.read(r.mar);

    co_return 0;
}

ModeTask ABX::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Low byte of address is fetched */
    r.mar = cpu.read(r.pc++);
    co_yield 0;

    /* High byte of address is fetched */
    r.mar |= cpu.read(r.pc++) << 8u;
    r.mar += r.x;
    co_yield 0;

    /* Fix high address byte */
    if (r.mar >> 8u != r.mar - r.x >> 8u)
        co_yield 1;

    /* Read from effective address */
    r.mdr = cpu.read(r.mar);

    co_return 0;
}

ModeTask ABY::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Low byte of address is fetched */
    r.mar = cpu.read(r.pc++);
    co_yield 0;

    /* High byte of address is fetched */
    r.mar |= cpu.read(r.pc++) << 8u;
    r.mar += r.y;
    co_yield 0;

    /* Fix high address byte */
    if (r.mar >> 8u != r.mar - r.y >> 8u)
        co_yield 1;

    r.mdr = cpu.read(r.mar);
    
    co_return 0;
}

ModeTask IND::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Low byte of address is fetched to MAR*/
    r.mar = cpu.read(r.pc++);
    co_yield 0;

    /* High byte of address is fetched to MAR */
    r.mar |= cpu.read(r.pc++) << 8u;
    co_yield 0;

    /* Fetch low byte to MDR */
    r.mdr = cpu.read(r.mar);
    co_yield 0;

    /* Fetch high byte to MAR */
    r.mar = cpu.read((r.mar & 0xFF00u) | ((r.mar + 1u) & 0x00FFu)) << 8u;
    r.mar |= r.mdr;

    co_return 0;

}

ModeTask IDX::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch pointer address to MAR */
    r.mar = cpu.read(r.pc++);
    co_yield 0;

    /* Add X to MAR */
    r.mar += r.x;
    co_yield 0;

    /* Fetch low address byte */
    r.mdr = cpu.read(r.mar & 0x00FFu);
    co_yield 0;

    /* Fetch high address byte */
    r.mar = cpu.read((r.mar + 1u) & 0x00FFu) << 8u;
    r.mar |= r.mdr;
    co_yield 0;

    /* Read from effective address */
    r.mdr = cpu.read(r.mar);

    co_return 0;
}

ModeTask IDY::execute(CPU &cpu, CPURegister &r) const
{
    /* Tick after fetching opcode */
    co_yield 0;

    /* Fetch pointer address to MAR */
    r.mar = cpu.read(r.pc++);
    co_yield 0;

    /* Fetch low address byte from zero page */
    r.mdr = cpu.read(r.mar & 0x00FFu);
    co_yield 0;

    /* Fetch high address byte from zero page */
    r.mar = cpu.read((r.mar + 1u) & 0x00FFu) << 8u;
    r.mar |= r.mdr;

    /* Add Y to low byte of effective address */
    r.mar += r.y;
    co_yield 0;

    /* *Fix* high byte of address (take extra cycle if page is crossed) */
    if (r.mar >> 8u != r.mar - r.y >> 8u)
        co_yield 1;

    /* Read from effective address */
    r.mdr = cpu.read(r.mar);
    
    co_return 0;
}