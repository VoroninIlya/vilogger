cmake_minimum_required(VERSION 3.22)

project(tst_vilogger)

#include(CodeCoverage)
include(CTest)

if(PROJECT_IS_TOP_LEVEL)
    if(NOT DEFINED UNITY_ROOT_PATH)
        set(UNITY_ROOT_PATH "../unity")
    endif()

    if(NOT DEFINED GTEST_ROOT_PATH)
        set(GTEST_ROOT_PATH "../gtest")
    endif()


    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/modules/unity)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/modules/gtest)
    # Set as executable
    add_subdirectory(${UNITY_ROOT_PATH} ${CMAKE_BINARY_DIR}/modules/unity)
    add_subdirectory(${GTEST_ROOT_PATH} ${CMAKE_BINARY_DIR}/modules/gtest)
endif()

# Add core subdir
add_subdirectory(core)

add_executable(tst_vilogger)

enable_testing()

# Add key executable block
target_sources(tst_vilogger PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/tests/main/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tests/tst_vilogger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tests/tst_viloglevel.cpp
)

# Add key include paths
target_include_directories(tst_vilogger PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/core/src/inc
    ${CMAKE_CURRENT_LIST_DIR}/tests/mock
    ${UNITY_ROOT_PATH}/src
    ${UNITY_ROOT_PATH}/extras/fixture/src
    ${UNITY_ROOT_PATH}/extras/memory/src
    ${GTEST_ROOT_PATH}/googlemock/include
)

# Compilation definition information
target_compile_definitions(tst_vilogger PUBLIC
    #WIN32
    #_DEBUG
    #CONSOLE
)

# Compiler options
target_compile_options(tst_vilogger PUBLIC
    -g
    -fprofile-arcs 
    -ftest-coverage
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(
  tst_vilogger PUBLIC vilogger unity gmock -lgcov -fprofile-arcs -ftest-coverage -pthread)

add_test(NAME ViLoggerTest COMMAND tst_vilogger "--gtest_filter=ViLoggerTest.*")
add_test(NAME ViLogLevelTest COMMAND tst_vilogger "--gtest_filter=ViLogLevelTest.*")
