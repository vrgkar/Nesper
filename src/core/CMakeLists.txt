target_sources(Nesper PRIVATE nes.cpp)

add_subdirectory(apu)
add_subdirectory(bus)
add_subdirectory(mapper)
add_subdirectory(cpu)
add_subdirectory(ppu)
add_subdirectory(utils)

target_include_directories(Nesper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})