#include "clock.h"

void Clock::tick(CPU *cpu)
{

    

    m_callback[cpu]();
}

void Clock::tick(PPU *ppu)
{

    

    m_callback[ppu]();
}

void Clock::tick(APU *apu)
{

    

    m_callback[apu]();
}