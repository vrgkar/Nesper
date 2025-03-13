#include "nes.h"

CPU NES::m_cpu;
PPU NES::m_ppu;
APU NES::m_apu;
std::shared_ptr<Mapper> NES::m_map;
Bus NES::m_bus;

bool NES::load_rom(std::string file)
{
    auto parser = ParserFactory::create_parser(file);
    ROM rom;
    parser->load(rom, file);

    m_map = MapperFactory::create_mapper(rom);
    m_map->load_rom(rom);

    m_cpu.connect(&m_bus);
    m_ppu.connect(&m_bus);
    m_apu.connect(&m_bus);
    m_map->connect(&m_bus);
    
    return true;
}

bool NES::step()
{
    INIT_SEGMENTS
    
    START_SEGMENT(1)
    m_bus.step();
    m_cpu.step();
    NEXT_SEGMENT(2)
    m_ppu.step();
    NEXT_SEGMENT(3)
    m_ppu.step();
    END_SEGMENT
    m_ppu.step();

    FREE_SEGMENTS
    return true;
}

unsigned char* NES::get_framebuffer()
{
    return m_ppu.framebuffer();
}

void NES::step_next_instruction()
{
    m_cpu.step_instruction();
}

std::string NES::get_current_instr_disassembly()
{
    return m_cpu.get_instr_disassembly();
}

bool NES::frame_ready()
{
    return m_ppu.frame_ready();
}