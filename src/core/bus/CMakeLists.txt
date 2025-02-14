add_library(bus bus.cpp)
target_link_libraries(bus PRIVATE cpu ppu apu ram mapper)