#include "nes.h"

CPU NES::m_cpu;
PPU NES::m_ppu;
APU NES::m_apu;
RAM NES::m_ram;
std::shared_ptr<Mapper> NES::m_map;
Bus NES::m_bus;

void NES::load_rom(std::string file)
{
    auto parser = ParserFactory::create_parser(file);
    ROM rom;
    parser->load(rom, file);

    m_map = MapperFactory::create_mapper(rom);
    m_map->load_rom(rom);
    m_bus.connect(m_map.get());
    m_bus.connect(&m_cpu);
    m_bus.connect(&m_ppu);
    m_bus.connect(&m_apu);
    m_bus.connect(&m_ram);

    m_cpu.set_bus(&m_bus);
}

void NES::step_next_instruction()
{
    m_cpu.step_instruction();
}

std::string NES::get_current_instr_disassembly()
{
    return m_cpu.get_instr_disassembly();
}

std::string NES::rom_get_disassembly()
{
    return "";
}