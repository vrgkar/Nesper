include_directories(core/)


add_subdirectory(core)
target_link_libraries(Nesper PRIVATE core)