cmake_minimum_required(VERSION 3.25)

project(
Nesper
VERSION 0.1 
LANGUAGES CXX)

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(wxWidgets REQUIRED COMPONENTS net core base richtext)
include(${wxWidgets_USE_FILE})

add_subdirectory(src bin)

add_executable(Nesper src/main.cpp)
target_link_libraries(Nesper PUBLIC core gui)
