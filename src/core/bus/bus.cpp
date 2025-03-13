#include "bus.h"

#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "apu/apu.h"
#include "mapper/mapper.h"

uint8_t Bus::read_cpu(uint16_t addr)
{
    uint8_t byte = 0;

    if (addr < 0x2000)
        byte = m_ram[addr % sizeof(m_ram)];
    else if (addr < 0x4000)
        byte = m_ppu->read(addr);
    else if (addr < 0x4014 || addr == 0x4015 || addr == 0x4017)
        byte = m_apu->read(addr);
    else if (addr == 0x4014 || addr == 0x4016)
        byte = m_cpu->read(addr);
    else if (addr >= 0x4020u)
        byte = m_mapper->read(addr);

    return byte;
}

void Bus::write_cpu(uint8_t byte, uint16_t addr)
{
    if (addr < 0x2000)
        m_ram[addr % sizeof(m_ram)] = byte; 
    else if (addr < 0x4000)
        m_ppu->write(byte, addr);
    else if (addr < 0x4014 || addr == 0x4015 || addr == 0x4017)
        m_apu->write(byte, addr);
    else if (addr == 0x4014 || addr == 0x4016)
        m_cpu->write(byte, addr);
    else if (addr >= 0x4020u)
        m_mapper->write(byte, addr);
}

uint8_t Bus::read_ppu(uint16_t addr)
{
    return m_mapper->read(addr);
}

void Bus::write_ppu(uint8_t byte, uint16_t addr)
{
    m_mapper->write(byte, addr);
}

void Bus::broadcast(void *component, Event event)
{
    if (component != m_cpu)
        m_cpu->push_event(event);
    if (component != m_ppu)
        m_ppu->push_event(event);
    if (component != m_apu)
        m_apu->push_event(event);
    if (component != m_mapper)
        m_mapper->push_event(event);
    if (component != this)
        m_events.push_back(event);
}

void Bus::process_events()
{
    for (auto event : m_events)
    {
        switch (event)
        {
        case Event::BRK: break;
        case Event::IRQ: break;
        case Event::NMI: break;
        case Event::OAMDMA: m_state = State::OAMDMA; break;
        case Event::NONE: break;
        }
    }

    m_events.clear();
}

void Bus::step()
{
    process_events();

    bool done = false;

    switch (m_state)
    {
    case State::IDLE:       done = true;        break;
    case State::OAMDMA:     done = OAMDMA();    break;
    case State::DMCDMA:     done = true;        break;
    }

    if (done)
        m_state = State::IDLE;
}

bool Bus::OAMDMA()
{
    INIT_SEGMENTS

    START_SEGMENT(1)

    m_cpu->suspend();
    m_mar = read_cpu(0x4014) << 8;
    m_mdr = read_cpu(m_mar++);

    NEXT_SEGMENT(2)

    write_cpu(m_mdr, 0x2004);

    if ((m_mar & 0x00FF) == 0)
    {
        m_cpu->resume();
        FREE_SEGMENTS
        return true;
    }

    END_SEGMENT

    m_mdr = read_cpu(m_mar++);
    GOTO_SEGMENT(2)

    return false;
}