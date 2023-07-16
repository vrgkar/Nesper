#include "cartridge.h"
#include <iostream>

void Cartridge::load(const ROM &rom)
{
    m_memory.load(rom);
    m_mapper = MapperFactory::create_mapper(rom);

    if (!m_mapper)
    {
        std::cout << "Failed to load mapper!" << std::endl;
    }
}

bool Cartridge::fetch(uint8_t &byte, uint16_t addr)
{
    return m_mapper->read(m_memory, byte, addr);
}

bool Cartridge::commit(uint8_t byte, uint16_t addr)
{
    return m_mapper->write(m_memory, byte, addr);
}

void Cartridge::service(Event event)
{
    
}