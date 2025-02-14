#include "cpu.h"

bool CPU::RST()
{
    co_yield 0;
    co_yield 0;
    co_yield 0;
    co_yield 0;
    co_yield 0;
    
    s.r.pc = bus.cpu_read(0xFFFCu);

    co_yield 0;

    s.r.pc |= bus.cpu_read(0xFFFDu) << 8u;

    co_return 0;
}

bool CPU::IRQ()
{
    co_return 0;
}

bool CPU::NMI()
{
    co_return 0;
}
