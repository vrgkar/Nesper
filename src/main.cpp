#include "core/cpu/cpu.h"
#include "core/bus/bus.h"

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include "core/utils/parser/parser.h"
#include "core/utils/parser/parser_factory.h"

#include "core/cpu/cpu.h"
#include "core/ppu/ppu.h"
#include "core/apu/apu.h"
#include "core/ram/ram.h"
#include "core/cartridge/cartridge.h"

int main(int argc, char **argv)
{
    std::ifstream handle("../roms/nestest.nes", std::ios::binary);
    auto size = std::filesystem::file_size("../roms/nestest.nes");

    std::string file;
    file.reserve(size);
    file.assign((std::istreambuf_iterator<char>(handle)), (std::istreambuf_iterator<char>()));

    auto parser = ParserFactory::create_parser(file);
    ROM rom;

    parser->load(rom, file);

    Cartridge cart;

    CPU cpu;
    PPU ppu;
    APU apu;
    RAM ram;

    cart.load(rom);

    Bus bus;
    bus.connect(&cpu);
    bus.connect(&ram);
    bus.connect(&apu);
    bus.connect(&ppu);
    bus.connect(&cart);

    cpu.set_bus(&bus);

    cpu.m_r.pc = 0xC000u;

    while (true)
        cpu.step();
}
