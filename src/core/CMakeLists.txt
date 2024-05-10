add_library(core nes.cpp)
add_library(system component.cpp)

add_subdirectory(apu)
add_subdirectory(bus)
add_subdirectory(cartridge)
add_subdirectory(cpu)
add_subdirectory(ppu)
add_subdirectory(ram)
add_subdirectory(utils)

target_include_directories(core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core PUBLIC system apu bus cartridge cpu ppu ram utils)
