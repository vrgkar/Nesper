add_library(cpu 
    cpu.cpp 
    cpuregister/cpuregister.cpp 
    opcode/opcode.cpp 
    mode/mode.cpp 
    state/state.cpp 
    interrupt/interrupt.cpp 
    )

target_link_libraries(cpu PUBLIC system bus)