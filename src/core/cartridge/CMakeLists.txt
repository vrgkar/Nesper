add_library(cartridge 
    cartridge.cpp 
    memory/memory.cpp 
    mapper/mapper.cpp 
    mapper/mapper_factory.cpp
    mapper/mapper000.cpp)
target_link_libraries(cartridge PUBLIC system)