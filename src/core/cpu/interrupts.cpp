#include "cpu.h"

bool CPU::RST()
{
    INIT_SEGMENTS

    START_SEGMENT(1)

    NEXT_SEGMENT(2)
    NEXT_SEGMENT(3)
    NEXT_SEGMENT(4)
    NEXT_SEGMENT(5)
    NEXT_SEGMENT(6)
    
    s.r.pc = bus.cpu_read(0xFFFCu);

    END_SEGMENT

    s.r.pc |= bus.cpu_read(0xFFFDu) << 8u;

    FREE_SEGMENTS
    return true;
}

bool CPU::IRQ()
{
    return true;
}

bool CPU::NMI()
{
    return true;
}
