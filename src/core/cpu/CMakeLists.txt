add_library(cpu 
    cpuregister.cpp 
    cpustate.cpp 
    instructions.cpp
    interrupts.cpp
    modes.cpp
    states.cpp
    cpu.cpp 
    )

target_link_libraries(cpu PUBLIC bus)