add_library(core nes.cpp)

add_subdirectory(apu)
add_subdirectory(bus)
add_subdirectory(mapper)
add_subdirectory(cpu)
add_subdirectory(ppu)
add_subdirectory(ram)
add_subdirectory(utils)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core PUBLIC apu bus mapper cpu ppu ram utils)
