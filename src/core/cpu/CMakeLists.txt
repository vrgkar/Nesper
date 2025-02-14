add_library(cpu 
    cpuregister.cpp 
    cpustate.cpp 
    opcode.cpp 
    mode.cpp 
    interrupt.cpp 
    cpu.cpp 
    instructions.cpp
    interrupts.cpp
    modes.cpp
    states.cpp
    )

target_link_libraries(cpu PUBLIC bus)