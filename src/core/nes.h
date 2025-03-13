#pragma once

#include "common/pch.h"

#include "bus/bus.h"
#include "apu/apu.h"
#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "mapper/mapper.h"
#include "mapper/mapper_factory.h"
#include "utils/parser.h"
#include "utils/parser_factory.h"

class NES
{
public:
    NES() = delete;
    void operator=(const NES&) = delete;

    static bool step();
    static void step_next_instruction();
    static std::string get_current_instr_disassembly();
    static bool load_rom(std::string rom);
    static unsigned char* get_framebuffer();
    static bool frame_ready();

    /*
     * Get the disassembly of the currently loaded ROM 
     */

    static std::string rom_get_disassembly();

private:

    static CPU m_cpu;
    static PPU m_ppu;
    static APU m_apu;
    static std::shared_ptr<Mapper> m_map;
    static Bus m_bus;
};