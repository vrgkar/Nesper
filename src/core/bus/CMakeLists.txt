add_library(bus bus.cpp)
target_link_libraries(bus PUBLIC cpu ppu apu ram cartridge)