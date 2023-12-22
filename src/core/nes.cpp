#include "nes.h"

void NES::service()
{
    auto p = m_events.top();

    if (&m_cpu != p.second)
        m_cpu.service(p.first);
    if (&m_ppu != p.second)
        m_ppu.service(p.first);
    if (&m_apu != p.second)
        m_apu.service(p.first);
    if (&m_cart != p.second)
        m_cart.service(p.first);

    switch (p.first)
    {
        //TO DO: Handle Events
    }

    m_events.pop();
}
