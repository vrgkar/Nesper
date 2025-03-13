target_sources(
    Nesper PRIVATE 
    instructions.cpp
    interrupts.cpp
    modes.cpp
    states.cpp
    cpu.cpp 
)

# target_link_libraries(cpu PUBLIC bus)