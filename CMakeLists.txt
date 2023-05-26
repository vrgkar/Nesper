cmake_minimum_required(VERSION 3.25)

project(
Nesper
VERSION 0.1 
LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# find_package(wxWidgets REQUIRED COMPONENTS net core base)
# find_package(SDL2 REQUIRED)

# include(${wxWidgets_USE_FILE})

add_executable(Nesper src/main.cpp)
add_subdirectory(src bin)