add_library(core nes.cpp)

add_subdirectory(system)
add_subdirectory(apu)
add_subdirectory(bus)
add_subdirectory(cartridge)
add_subdirectory(cpu)
add_subdirectory(ppu)
add_subdirectory(ram)
add_subdirectory(utils)

target_link_libraries(core PUBLIC system apu bus cartridge cpu ppu ram utils)
