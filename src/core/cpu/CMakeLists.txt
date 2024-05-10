add_library(cpu 
    cpu.cpp 
    cpuregister.cpp 
    opcode.cpp 
    mode.cpp 
    state.cpp 
    interrupt.cpp 
    )

target_link_libraries(cpu PUBLIC system bus)