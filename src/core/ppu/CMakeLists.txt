add_library(ppu ppu.cpp ppuregister/ppuregister.cpp)
target_link_libraries(ppu PUBLIC system)