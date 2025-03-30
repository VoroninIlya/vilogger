cmake_minimum_required(VERSION 3.22)

project(vilogger)

# Debug message
message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

# Register core library
add_library(vilogger INTERFACE)

target_sources(vilogger PUBLIC src/vilogger.cpp src/viloglevel.cpp)

target_include_directories(vilogger INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/inc)
