add_library(cpu 
    cpuregister.cpp 
    cpustate.cpp 
    opcode.cpp 
    mode.cpp 
    interrupt.cpp 
    cpu.cpp 
    )

target_link_libraries(cpu PUBLIC bus)