#include "interrupt.h"

Task RST::execute(Bus &bus, CPUState &s) const
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

Task IRQ::execute(Bus &bus, CPUState &s) const
{
    co_return 0;
}

Task NMI::execute(Bus &bus, CPUState &s) const
{
    co_return 0;
}
