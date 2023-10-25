cmake_minimum_required(VERSION 2.8)

project (young-test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "-O2 -lstdc++")

include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(src YTEST)
aux_source_directory(test TEST_CASES)

add_executable(test ${TEST_CASES} ${YTEST})

set(CMAKE_VERBOSE_MAKEFILE false)
